
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>

#define ERROR(e) { perror(e); exit(EXIT_FAILURE); }
#define SERVER_PORT 1234
#define QUEUE_SIZE 5



char* getFilePath(char *path, int offset) {
  int i = offset;
  char *output = malloc (sizeof (char) * 100);
  while (path[i] != ' ') {
    output[i - offset] = path[i];
    i++;
  }
  return output;
}

char* concat(char* str1, char* str2) {
  char * output = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
  strcpy(output, str1);
  strcat(output, str2);
  return output;
}

int isBlank(char *line) {
  char* ch;
  int isBlankLine = 1;
  printf("%s\n", line);
  for (ch = line; *ch != '\0'; ++ch)
  {
    if (!isspace(*ch))
    {
      isBlankLine = 0;
      break;
    }
  }

  return isBlankLine;
}

int checkIfFileExist(char* fileName) {
  if( access( fileName, F_OK ) != -1 ) {
    return 1;
  } else {
    return 0;
  }
}

int getFileSize(FILE* fp) {
  fseek(fp, 0, SEEK_END);
  int fileSize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return fileSize;
}

int main(int argc, char** argv) {
  char* not_found_response_template = 
      "HTTP/1.1 404 Not Found\n"
      "Content-type: text/html\n"
      "\n"
      "<html>\n"
      " <body>\n"
      "  <h1>Not Found</h1>\n"
      "  <p>The requested URL was not found on this server.</p>\n"
      " </body>\n"
      "</html>\n";

  int not_found_response_template_len = strlen(not_found_response_template);

  char* DEFAULT_RESPONSE_SUCCESS_TEMPLATE = 
      "HTTP/1.1 200 Success\n";
  int DEFAULT_RESPONSE_SUCCESS_TEMPLATE_LEN = strlen(DEFAULT_RESPONSE_SUCCESS_TEMPLATE);

  char* METHOD_NOT_ALLOWED = 
      "HTTP/1.1 405 Method Not Allowed\n";
  int METHOD_NOT_ALLOWED_LEN = strlen(METHOD_NOT_ALLOWED);

	char buf[4096];
  socklen_t slt;
  int sfd, cfd, fdmax, fda, rc, i, on = 1;
  struct sockaddr_in saddr, caddr;
  static struct timeval timeout;
  fd_set mask, rmask, wmask;

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = INADDR_ANY;
  saddr.sin_port = htons(SERVER_PORT);

  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    ERROR("socket()")
  if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0)
    ERROR("setsockopt()")
  if (bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
    ERROR("bind()")
  if (listen(sfd, QUEUE_SIZE) < 0)
    ERROR("listen()")

  FD_ZERO(&mask);
  FD_ZERO(&rmask);
  FD_ZERO(&wmask);
  fdmax = sfd;

  while(1) {
    FD_SET(sfd, &rmask);
    wmask = mask;
    timeout.tv_sec = 5 * 60;
    timeout.tv_usec = 0;
    if ((rc = select(fdmax+1, &rmask, &wmask, (fd_set*)0, &timeout)) < 0)
      ERROR("select()")

    if (rc == 0) {
      printf("timed out\n");
      continue;
    }

    fda = rc;
    if (FD_ISSET(sfd, &rmask)) {
      fda -= 1;
      slt = sizeof(caddr);
      if ((cfd = accept(sfd, (struct sockaddr*)&caddr, &slt)) < 0)
        ERROR("accept()")
      printf("new connection: %s\n",
             inet_ntoa((struct in_addr)caddr.sin_addr));
      FD_SET(cfd, &mask);
      if (cfd > fdmax) fdmax = cfd;
    }


    for (i = sfd+1; i <= fdmax && fda > 0; i++)
      if (FD_ISSET(i, &wmask)) {
        fda -= 1;
    		read(i, buf, 2047);
    		if (strncmp(buf, "GET /", 5) == 0) {
          char* filePath = concat("./", getFilePath(buf, 5));
          if (checkIfFileExist(filePath) == 1) {
      			FILE *fp;
            fp = fopen(filePath, "r");
            int fileSize = getFileSize(fp);
            char *fileContent = malloc(fileSize + 1);
            fread(fileContent, fileSize, 1, fp);
            fclose(fp);
            write(i, fileContent, fileSize);
          } else {
            write(i, not_found_response_template, not_found_response_template_len);
          }
    		} else if (strncmp(buf, "HEAD /", 6) == 0) {
          char* filePath = concat("./", getFilePath(buf, 6));
          if (checkIfFileExist(filePath) == 1) {
            write(i, DEFAULT_RESPONSE_SUCCESS_TEMPLATE, DEFAULT_RESPONSE_SUCCESS_TEMPLATE_LEN);
          } else {
            write(i, not_found_response_template, not_found_response_template_len);
          }
          write(i, "HEAD!\n", 6);
        } else if (strncmp(buf, "PUT /", 5) == 0) {
          char* filePath = concat("./", getFilePath(buf, 5));
          FILE *fp;
          fp = fopen(filePath, "w");
          int i = 0;
          int j = 0;
          int shouldWrite = 0;
          int inputLength = strlen(buf);
          char fileContentToSave[2047];
          char tempLine[2047];
          while (j < inputLength) {
            if (shouldWrite == 1) {
              fileContentToSave[i] = buf[j];
              i++;
              j++;

            } else {
              while(buf[j] != '\n') {
                tempLine[i] = buf[j];
                j++;
                i++;
              }
              if (isBlank(tempLine) == 1) {
                shouldWrite = 1;
              }
              j++;
              i = 0;
              memset(tempLine, 0, sizeof tempLine);
            }
          }
          fclose(fp);
          write(i, DEFAULT_RESPONSE_SUCCESS_TEMPLATE, DEFAULT_RESPONSE_SUCCESS_TEMPLATE_LEN);
        } else if (strncmp(buf, "DELETE /", 8) == 0) {
          char* filePath = concat("./", getFilePath(buf, 8));
          if (checkIfFileExist(filePath) == 1) {
            int successfulDelete = remove(filePath);
            if (successfulDelete == 0) {
              write(i, DEFAULT_RESPONSE_SUCCESS_TEMPLATE, DEFAULT_RESPONSE_SUCCESS_TEMPLATE_LEN);
            }
          } else {
            write(i, not_found_response_template, not_found_response_template_len);
          }
        } else {
          write(i, METHOD_NOT_ALLOWED, METHOD_NOT_ALLOWED_LEN);
  			}
        close(i);
        FD_CLR(i, &mask);
        if (i == fdmax)
          while(fdmax > sfd && !FD_ISSET(fdmax, &mask))
            fdmax -= 1;
      }
  }

  close(sfd);
  return EXIT_SUCCESS;
}

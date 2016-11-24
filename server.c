
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

int checkIfFileExist(char* fileName) {
  if( access( fileName, F_OK ) != -1 ) {
    return 1;
  } else {
    return 0;
  }
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
          if (checkIfFileExist(concat("./", getFilePath(buf, 5))) == 1) {
      			FILE *fp;
            fp = fopen(concat("./", getFilePath(buf, 5)), "r");
            fseek(fp, 0, SEEK_END); // seek to end of file
            int fileSize = ftell(fp); // get current file pointer
            fseek(fp, 0, SEEK_SET); // seek back to beginning of file
            printf("file size: %d\n", fileSize);
            char *fileContent = malloc(fileSize + 1);
            fread(fileContent, fileSize, 1, fp);
            fclose(fp);
            printf("%s\n", "GET");
            write(i, fileContent, fileSize);
          } else {
            write(i, not_found_response_template, not_found_response_template_len);
          }
    		} else if (strncmp(buf, "HEAD /", 5) == 0) {
          printf("%s\n", "HEAD");
          write(i, "HEAD!\n", 6);
        } else if (strncmp(buf, "PUT /", 5) == 0) {
          write(i, "PUT!\n", 5);
          printf("%s\n", "PUT");
        } else if (strncmp(buf, "DELETE /", 5) == 0) {
          printf("%s\n", "DELETE");
          write(i, "DELETE!\n", 6);
        } else {
          write(i, "Wrong type of request!\n", 6);
          printf("%s\n", "Wrong type of request");
  			}
        write(i, "Hello World!\n", 13);
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

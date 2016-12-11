
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
#include "helpers.h"
#include "templates.h"

#define ERROR(e) { perror(e); exit(EXIT_FAILURE); }
#define SERVER_PORT 1234
#define QUEUE_SIZE 5

int main(int argc, char** argv) {
	char buf[4096];
  int not_found_response_template_len = strlen(not_found_response_template);
  int DEFAULT_RESPONSE_SUCCESS_TEMPLATE_LEN = strlen(DEFAULT_RESPONSE_SUCCESS_TEMPLATE);
  int METHOD_NOT_ALLOWED_LEN = strlen(METHOD_NOT_ALLOWED);

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
        int iter = 0;
        int numberOfBytesRead = -1;
        while(!isEndOfHeader(buf)) {
          numberOfBytesRead = read(i, buf + iter, 1);
          iter += numberOfBytesRead;
        }
        // GET

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
            free(fileContent);
          } else {
            write(i, not_found_response_template, not_found_response_template_len);
          }

        // HEAD

    		} else if (strncmp(buf, "HEAD /", 6) == 0) {
          char* filePath = concat("./", getFilePath(buf, 6));
          if (checkIfFileExist(filePath) == 1) {
            write(i, DEFAULT_RESPONSE_SUCCESS_TEMPLATE, DEFAULT_RESPONSE_SUCCESS_TEMPLATE_LEN);
          } else {
            write(i, not_found_response_template, not_found_response_template_len);
          }
          write(i, "HEAD!\n", 6);
        
        // PUT

        } else if (strncmp(buf, "PUT /", 5) == 0) {
          char* filePath = concat("./", getFilePath(buf, 5));
          int fileContentIndex = 0;
          int contentLength = getContentLength(buf);
          FILE *fp;
          fp = fopen(filePath, "w");
          char *fileContentToSave = malloc(contentLength + 1);
          while (fileContentIndex < contentLength) {
            read(i, fileContentToSave + fileContentIndex, 1);
            fileContentIndex++;
          }
          fputs(fileContentToSave, fp);
          fclose(fp);
          free(fileContentToSave);
          write(i, DEFAULT_RESPONSE_SUCCESS_TEMPLATE, DEFAULT_RESPONSE_SUCCESS_TEMPLATE_LEN);

        // DELETE  

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
        // clearing
        memset(buf, 0, sizeof buf);
        iter = 0;
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

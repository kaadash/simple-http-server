#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct sockaddr_in sockAddr, clientSockAddr;
char buf[4096];
const int QUEUE_SIZE = 10;
void initSockAddr() {
	sockAddr.sin_family = PF_INET;	
	sockAddr.sin_port = htons(8080);
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

int main(int argc, char* argv[]) {
	initSockAddr();
	int tmpSizeOfFileDescriptor;

	// socket
	int clientSocketFileDescriptor;
	int serverSocketFileDescriptor = socket(PF_INET, SOCK_STREAM, 0);
	if (serverSocketFileDescriptor < 0) {
		perror("socket");
		exit(1);
	}

	// bind
	if (bind(serverSocketFileDescriptor, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr)) < 0) {
		perror("bind");
		close(serverSocketFileDescriptor);
		exit(1);
	}

	// listen
	if (listen(serverSocketFileDescriptor, QUEUE_SIZE) < 0) {
		perror("listen");
		close(serverSocketFileDescriptor);
		exit(1);
	}

	// accept
	while(1) {
		tmpSizeOfFileDescriptor = sizeof(struct sockaddr);
		clientSocketFileDescriptor = accept(serverSocketFileDescriptor, (struct sockaddr*)&clientSockAddr, &tmpSizeOfFileDescriptor);
		if (clientSocketFileDescriptor < 0) {
			perror("accept");
		}
		printf("%s\n", "Connection with server");
		if (!fork()) {
			read(clientSocketFileDescriptor, buf, 2047);
			if (strncmp(buf, "GET /index.html", 16)) {
				write(clientSocketFileDescriptor, "<!DOCTYPE html><html><head><title>test server</title></head><body><h1>Testing...</h1></body></html>", 109);
			}
			close(clientSocketFileDescriptor);
			printf("%s\n", "Closing connection...");
			exit(0);
		}
	}

	close(serverSocketFileDescriptor);

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include "dynsrv.h"

int main(int argc, char *argv[]) {
	int sockfd;
	int status;
	char * source_addr;
	pid_t child, pid;
	source_addr = (char *) malloc(16 * sizeof(char));
	bzero(source_addr, 16 * sizeof(char));

	if(argc < 2) {
		fprintf(stderr, "ERROR: no port provided.\n");
		exit(1);
	}
	sockfd = bindToInterface(atoi(argv[1]));
	if(sockfd < 0) {
		fprintf(stderr, "Cannot bind to interface\n");
		exit(1);
	}
	child = fork();
	if(!child) {
		if(clientConn(sockfd, source_addr) < 0) {
			fprintf(stderr, "connection failed\n");
			exit(1);
		}
		else
			printf("Connection from: %s\n", source_addr);
	}
	pid = wait(&status);
	printf("Proces: %d zakonczyl sie z kodem: %d\n", pid, WEXITSTATUS(status));
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "dynsrv.h"

int main(int argc, char *argv[]) {
	int sockfd, cli_fd;
	int status;
	int n;
	char * source_addr;
	char * client_domain;
	pid_t child, pid;
	source_addr = (char *) malloc(16 * sizeof(char));
	client_domain = (char *) malloc(64 * sizeof(char));
	bzero(source_addr, 16 * sizeof(char));
	bzero(client_domain, 64 * sizeof(char));

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
		if((cli_fd = clientConn(sockfd, source_addr)) < 0) {
			fprintf(stderr, "connection failed\n");
			exit(1);
		}
		else {
			printf("Connection from: %s\n", source_addr);
			n = readData(cli_fd, client_domain);
			printf("Domena: %s\n", client_domain);
		}
	}
	pid = wait(&status);
	printf("Proces: %d zakonczyl sie z kodem: %d\n", pid, WEXITSTATUS(status));
	return 0;
}

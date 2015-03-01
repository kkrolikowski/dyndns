#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include "dynsrv.h"

static void stripSubDomain(char *str, char *sd);

int main(int argc, char *argv[]) {
	int sockfd, cli_fd;
	int status;
	char * source_addr;
	char * client_domain;
	pid_t child, pid;
	cfgdata_t cf;

	source_addr = (char *) malloc(16 * sizeof(char));
	client_domain = (char *) malloc(64 * sizeof(char));
	bzero(source_addr, 16 * sizeof(char));
	bzero(client_domain, 64 * sizeof(char));

	if(argc < 3) {
		fprintf(stderr, "Usage: %s [port number] <bind zonefile>\n", argv[0]);
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
			strcpy(cf.ip_addr, source_addr);
			if(readData(cli_fd, client_domain) > 0)
				stripSubDomain(client_domain, cf.subdomain);
			else {
				fprintf(stderr, "Error reading data from client\n");
				exit(1);
			}
		}
		if(!fork()) {
			printf("Connection from: %s\n", cf.ip_addr);
			printf("Domena: %s\n", cf.subdomain);
		}
		pid = wait(&status);
		printf("Proces: %d zakonczyl sie z kodem: %d\n", pid, WEXITSTATUS(status));
	}
	return 0;
}
static void stripSubDomain(char *str, char *sd) {
	while(*str != '\0') {
		if(*str == '.') {
			*sd = '\0';
			break;
		}
		else
			*sd = *str;
		str++;
		sd++;
	}
}

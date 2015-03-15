#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "config.h"

int main(int argc, char *argv[]) {
	config_t config;
	pid_t pid, sess;
	int sockfd, portno, n;
	struct sockaddr_in srv_addr;
	struct hostent *server;
	struct iovec authdata[2];
	char buffer[256];
	char ack[8];

	if(argc < 3) {
		fprintf(stderr, "%s -c <configfile>\n\t -h print this help\n", argv[0]);
		exit(1);
	}
	if(strcmp(argv[1], "-c") != 0) {
		fprintf(stderr, "%s -c <configfile>\n\t -h print this help\n", argv[0]);
		exit(1);
	}
	if(ReadCFG(&config, argv[2]) == false) {
		fprintf(stderr, "Error reading file: %s\n", argv[2]);
		exit(1);
	}
	pid = fork();
	if(pid < 0) {
		perror("fork");
		exit(-1);
	}
	if(pid > 0) {
		printf("Starting DynDNS client, PID: %d\n", pid);
		exit(0);
	}
	umask(0);
	sess = setsid();
	chdir("/");
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	portno = config.port;
	while(1) {
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0)
			perror("Error opening socket");
		server = gethostbyname(config.client.host);
		if(server == NULL) {
			fprintf(stderr, "Unknown host\n");
			exit(1);
		}
		bzero((char *) &srv_addr, sizeof(srv_addr));
		srv_addr.sin_family = AF_INET;
		bcopy((char *) server->h_addr, (char *) &srv_addr.sin_addr.s_addr, server->h_length);
		srv_addr.sin_port = htons(portno);
		if(connect(sockfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0) {
			fprintf(stderr, "Error connecting to server\n");
			exit(1);
		}

		authdata[0].iov_base = config.client.username;
		authdata[0].iov_len = strlen(config.client.username)+1;
		authdata[1].iov_base = config.client.password;
		authdata[1].iov_len = strlen(config.client.password)+1;

		n = writev(sockfd, authdata, 2);
		if(n < 0) {
			fprintf(stderr, "error sending authdata");
			exit(1);
		}
		n = read(sockfd, ack, 8);
		if(strcmp(ack, "authok") == 0) {
			bzero(buffer, 256);
			strcpy(buffer, config.client.domain);
			n = write(sockfd, buffer, strlen(buffer) +1);
			if(n < 0) {
				fprintf(stderr, "Error sending data.");
				exit(1);
			}
		}
		close(sockfd);
		sleep(config.client.interval);
	}
	return 0;
}

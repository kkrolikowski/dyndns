#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "dynsrv.h"
#include "config.h"

int main(int argc, char *argv[]) {
	pid_t pid, sess, dsrv;
	int log_fd, sockfd, status;
	extern char logmsg[LOG_MSG_LEN];
	extern char t_stamp[TIMESTAMP_LEN];
	config_t config;

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
		printf("Starting DynDNS service, PID: %d\n", pid);
		exit(0);
	}
	umask(0);
	sess = setsid();
	chdir("/");
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	log_fd = open(config.server.logfile, O_RDWR|O_CREAT|O_APPEND, 0644);
	sockfd = bindToInterface(config.server.port);
	if (sockfd < 0) {
		sprintf(logmsg, "%s ERROR: Cannot bind to interface\n", timestamp(t_stamp));
		write(log_fd, logmsg, strlen(logmsg));
		exit(1);
	}
	else {
		sprintf(logmsg, "%s INFO: Listening on port: %s\n", timestamp(t_stamp), argv[1]);
		write(log_fd, logmsg, strlen(logmsg));
	}
	while(1) {
		dsrv = fork();
		if(dsrv > 0)
			wait(&status);
		else if(dsrv == 0)
			ddserv(config.server.zonedir, log_fd, sockfd);
		else {
			perror("fork");
			exit(-1);
		}
	}
	close(log_fd);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "dynsrv.h"

int main(int argc, char *argv[]) {
	pid_t pid, sess, dsrv;
	int log_fd, sockfd, status;
	extern char logmsg[LOG_MSG_LEN];
	extern char t_stamp[TIMESTAMP_LEN];

	if (argc < 3) {
		fprintf(stderr, "Usage: %s [port number] <zonedir> <logfile>\n", argv[0]);
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
	log_fd = open(argv[3], O_RDWR|O_CREAT|O_APPEND, 0644);
	sockfd = bindToInterface(atoi(argv[1]));
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
			ddserv(argv[1], argv[2], log_fd, sockfd);
		else {
			perror("fork");
			exit(-1);
		}
	}
	close(log_fd);
	return 0;
}

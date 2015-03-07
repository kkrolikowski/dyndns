#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
	pid_t pid, sess;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s [port number] <zonedir>\n", argv[0]);
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
	while(1) {
		ddserv(argv[1], argv[2]);
	}

	return 0;
}

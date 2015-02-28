#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>

void error(char *msg);

int main(int argc, char *argv[]) {
        int sockfd, newsockfd, portno, clilen, n;
	int status;
        char buffer[256];
        struct sockaddr_in serv_addr, cli_addr;
	char * source_addr;
	pid_t child, pid;

        if(argc < 2) {
                fprintf(stderr, "ERROR: no port provided.\n");
                exit(1);
        }
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0)
                error("ERROR opening socket");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[1]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
                error("ERROR on binding");
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
	printf("PID: %d\n", (int) getpid());
	child = fork();
	if(!child) {
		printf("PID: %d, PPID: %d\n", (int) getpid(), (int)getppid());
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0)
			error("ERROR on accept");
		source_addr = (char *) malloc(16 * sizeof(char));
		if(source_addr != NULL) {
			source_addr = (char *) inet_ntoa(cli_addr.sin_addr.s_addr);
			printf("Connection from: %s\n", source_addr);
			exit(0);
		}
		else
			error("ERROR allocate memory");
	}
	pid = wait(&status);
	printf("Proces: %d zakonczyl sie z kodem: %d\n", pid, WEXITSTATUS(status));
        return 0;

}
void error(char *msg) {
        perror(msg);
        exit(1);
}

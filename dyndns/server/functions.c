#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include "dynsrv.h"

int bindToInterface(int portno) {
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("ERROR opening socket");
		return -1;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		return -1;
	}
	listen(sockfd, 5);

	return sockfd;
}
int clientConn(int fd, char * cliaddr) {
	int clilen, clifd;
	struct sockaddr_in cli_addr;

	clilen = sizeof(cli_addr);
	clifd = accept(fd, (struct sockaddr *) &cli_addr, &clilen);

	if(clifd < 0)
		return -1;
	else {
		strcpy(cliaddr, (char *) inet_ntoa(cli_addr.sin_addr));
		return clifd;
	}
}
int readData(int fd, char * domain) {
	char buf[256];
	int n;

	n = read(fd, buf, 255);
	if(n < 0) {
		fprintf(stderr, "Error reading from socket");
		return -1;
	}
	strcpy(domain, buf);
	return n;
}
int updateZone(cfgdata_t * cf, char * file) {
    FILE *zf;
    FILE *tmp;
    char tmp_path[21];
    char buf[256];
    char configline[256];
    int buflen, cfgline_len, maxval;
    long config_pos = 0;
    int n;

    RandomFilename(tmp_path);
    strcat(tmp_path);
    bzero(configline, 256);
    if(strlen(cf->subdomain) < 8)
    	sprintf(configline, "%s\t\tIN\tA\t%s", cf->subdomain, cf->ip_addr);
    else
    	sprintf(configline, "%s\tIN\tA\t%s", cf->subdomain, cf->ip_addr);
    cfgline_len = strlen(configline);
    zf = fopen(file, "r+");
    if(zf == NULL) {
            fprintf(stderr, "Error reading file: %s", file);
            return 1;
    }
    tmp = fopen(tmp_path, "w");
    if(tmp == NULL) {
    	fprintf(stderr, "Error creating file: %s", tmp_path);
    	return 1;
    }
    while(fgets(buf, sizeof(buf), zf) != NULL) {
            if(strstr(buf, "; dyndns") != NULL)
                    break;
    }
    config_pos = ftell(zf);
    while(fgets(buf, sizeof(buf), zf) != NULL) {
            if(strstr(buf, cf->subdomain) != NULL) {
            		config_pos = ftell(zf) - strlen(buf);
                    buflen = strlen(buf);
                    if(buflen > cfgline_len)
                            maxval = buflen;
                    else
                            maxval = cfgline_len;
                    fseek(zf, config_pos, SEEK_SET);
                    for(n=0; n < maxval; n++) {
                            if(configline[n] != '\0')
                                    fputc(configline[n], zf);
                            else
                                    fputc('\0', zf);
                    }
                    fputc('\n', zf);
            }
    }
    fclose(zf);
    fclose(tmp);

    return 0;
}
int updateSerialNo(char * oldserial, char * newserial) {
    time_t today;
    struct tm tf;
    char serial[12];
    char * ver = "00";
    long bigger_serial = 0;

    time(&today);
    localtime_r(&today, &tf);
    strftime(serial, sizeof(serial), "%Y%m%d", &tf);
    strcat(serial, ver);

    if(atol(oldserial) > atol(serial)) {
            bigger_serial = atol(oldserial) + 1;
            sprintf(serial, "%ld", bigger_serial);
            strcpy(newserial, serial);
            return 0;
    }
    else if(atol(oldserial) == atol(serial))
           return 1;
    else {
    	strcpy(newserial, serial);
    	return 0;
    }
}
void RandomFilename(char *filename) {
    char * entropy = "1qaz2wsx3edc4rfv5tgb6yhn7ujm8i0poklaZAQ1XSW2CDE3VFR4BGT5NHY6MJU6MJU7";
    int n, i;
    const int len = 8;                              // random part lenght
    struct timeval tv;
    char tmp[20] = "/tmp/dyndns_";                  // prefix of temp file
    char * ptmp;                                    // pointer to the begining of temp filename
    ptmp = tmp + strlen(tmp);                       // moving to the end of a prefix

    for(i=0; i<len; i++, ptmp++) {                  // while counter is smaller then lenght of random part
            gettimeofday(&tv, NULL);                // get current time
            srand(tv.tv_usec);                      // generate new seed with time in micro seconds
            n = rand() % strlen(entropy);           // generate new random character
            *ptmp = entropy[n];                     // and put it in the next field of an array.
    }
    *ptmp = '\0';                                   // end of string containing random filename
    strcpy(filename, tmp);
}

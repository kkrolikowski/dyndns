#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>
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
    char serial[12];
    char newserial[12];

    zf = fopen(file, "r");
    if(zf == NULL) {
            fprintf(stderr, "Error reading file: %s", file);
            return 1;
    }
    RandomFilename(tmp_path);
    tmp = fopen(tmp_path, "w");
    if(tmp == NULL) {
    	fprintf(stderr, "Error creating file: %s", tmp_path);
    	return 1;
    }
    while(fgets(buf, sizeof(buf), zf) != NULL) {
	    if(strstr(buf, "; serial") != NULL) {
		stripSerialNo(buf, serial);
		if(!updateSerialNo(serial, newserial))
			sprintf(buf, "\t%s\t; serial\n", newserial);
	    }
	    if(strstr(buf, cf->subdomain) != NULL) {
		    if(strlen(cf->subdomain) < 8)
		    	sprintf(buf, "%s\t\tIN\tA\t%s\n", cf->subdomain, cf->ip_addr);
		    else
		    	sprintf(buf, "%s\tIN\tA\t%s\n", cf->subdomain, cf->ip_addr);
	    }
	    fputs(buf, tmp);
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

    if(atol(oldserial) >= atol(serial)) {
            bigger_serial = atol(oldserial) + 1;
            sprintf(serial, "%ld", bigger_serial);
            strcpy(newserial, serial);
            return 0;
    }
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
void stripSerialNo(char *in, char *out) {
	char serial[12];
	char * pserial;

	pserial = serial;
	while(*in) {
		if(isdigit(*in)) {
			*pserial = *in;
			pserial++;
		}
		in++;
	}
	*in = '\0';
	strcpy(out, serial);
}
bool if_Exist(char *item, char *zfname) {
	FILE *zf;
	char buf[256];
	int found = 0;

	zf = fopen(zfname, "r");
	if(zf == NULL) {
		fprintf(stderr, "Error opening file: %s\n", zfname);
		return false;
	}
	while(fgets(buf, sizeof(buf), zf) != NULL) {
		if(strstr(buf,item) != NULL) {
			found = 1;
			break;
		}
	}
	fclose(zf);

	if(found)
		return true;
	else
		return false;
}
bool NewEntry(cfgdata_t * cf, char *fname) {
	FILE * zf;

	zf = fopen(fname, "a");
	if(zf == NULL) {
		fprintf(stderr, "Error opening file %s", fname);
		return false;
	}
    if(strlen(cf->subdomain) < 8)
    	fprintf(zf, "%s\t\tIN\tA\t%s\n", cf->subdomain, cf->ip_addr);
    else
    	fprintf(zf, "%s\tIN\tA\t%s\n", cf->subdomain, cf->ip_addr);

    fclose(zf);
    return true;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#define _XOPEN_SOURCE
#include <unistd.h>
#ifndef __FreeBSD__
#define _GNU_SOURCE
#include <crypt.h>
#endif
#include "common.h"
#include "auth.h"
#include "clientmanager.h"

int userauth(char * secret, char *pass) {
	char * salt;

	salt = (char *) malloc(16 * sizeof(char));

	get_salt(secret, salt);
	if(strcmp((char *) secret, (char *) crypt(pass, salt)) != 0) {
		free(salt);
		return 0;
	}
	free(salt);
	return 1;
}
void get_salt(char *p, char *salt) {
	int delim = 0;

	while(delim < 3) {
		if(*p == '$')
			delim++;
		*salt++ = *p++;
	}
	*salt = '\0';
}

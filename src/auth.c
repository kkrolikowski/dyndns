#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#ifndef __FreeBSD__
#include <shadow.h>
#endif
#include <sys/types.h>
#include <sys/uio.h>
#define _XOPEN_SOURCE
#include <unistd.h>
#ifndef __FreeBSD__
#define _GNU_SOURCE
#include <crypt.h>
#endif
#include "auth.h"

int userauth(char *login, char *pass) {
	char * epass;
	char * salt;
#ifndef __FreeBSD__
	struct spwd *pw;
#else
	struct passwd *pw;
#endif
	epass = (char *) malloc(36 * sizeof(char));
	salt = (char *) malloc(16 * sizeof(char));

#ifndef __FreeBSD__
	if((pw = getspnam(login)) == NULL) {
#else
	if((pw = getpwnam(login)) == NULL) {
#endif
		free(salt);
		free(epass);
		return 1;
	}
#ifndef __FreeBSD__
	get_salt(pw->sp_pwdp, salt);
	if(strcmp(pw->sp_pwdp, (char *)crypt(pass, salt)) != 0) {
#else
	get_salt(pw->pw_passwd, salt);
	if(strcmp(pw->pw_passwd, (char *)crypt(pass, salt)) != 0) {
#endif
		free(salt);
		free(epass);
		return 2;
	}
	free(epass);
	free(salt);

	return 0;
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
int isAuthorized(char *user, char *domain) {
	struct passwd * pw;

	if((pw = getpwnam(user)) == NULL)
		return -1;
	if(strcmp(domain, pw->pw_gecos) == 0)
		return 1;
	else
		return -1;
}

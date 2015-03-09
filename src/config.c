#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dyndns.h"

bool ReadCFG(PARAMS * cfg, char * filename) {
	FILE * cfgfile;
	char opt[50];
	char val[50];
	char buf[256];

	cfgfile = fopen(filename, "r");
	if(cfgfile == NULL)
		return false;
	while(fgets(buf, sizeof(buf), cfgfile) != NULL) {
		if(buf[0] == '#')
			continue;
		if(strstr(buf, "server =")) {
			printf("%s\n", getVal(buf));
			strcpy(cfg->host, getVal(buf));
		}
		if(strstr(buf, "port ="))
			cfg->port = atoi(getVal(buf));
		if(strstr(buf, "domain ="))
			strcpy(cfg->domain, getVal(buf));
	}
	fclose(cfgfile);
	return true;
}
char * getVal(char * str) {
	char * newstr = str;

	while(*str) {
		newstr = ++str;
		if(isspace(*str) && (isalpha(*(str+1)) || isdigit(*(str+1))))
			break;
	}
	str = newstr;
	return ++str;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dyndns.h"

bool ReadCFG(PARAMS * cfg, char * filename) {
	FILE * cfgfile;
	char opt[50];
	char val[50];

	cfgfile = fopen(filename, "r");
	if(cfgfile == NULL) {
		fprintf(stderr, "Error opening %s", filename);
		return false;
	}
	while(fscanf(cfgfile, "%s %s", opt, val) != EOF) {
		if(strcmp(opt, "server") == 0)
			strcpy(cfg->host, val);
		if(strcmp(opt, "port") == 0) {
			cfg->port = atoi(val);
		}
	}
	fclose(cfgfile);
	return true;
}

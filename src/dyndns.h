#ifndef DYNDNS_H_
#define DYNDNS_H_
#include <stdbool.h>

typedef struct userconfig {
	char host[50];
	char domain[64];
	int port;
} userconfig_t;
bool ReadCFG(userconfig_t * cfg, char * filename);
char * getVal(char * str);

#endif /* DYNDNS_H_ */

#ifndef DYNSRV_H_
#define DYNSRV_H_

struct cfgdata {
	char domain[64];
	char ip_addr[16];
} CFGDATA;

int bindToInterface(int portno);
int clientConn(int fd, char * cliaddr);
int readData(int fd, char * domain);
#endif

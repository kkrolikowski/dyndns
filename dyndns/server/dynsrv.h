#ifndef DYNSRV_H_
#define DYNSRV_H_
int bindToInterface(int portno);
int clientConn(int fd, char * cliaddr);
char * readData(int fd);
void error(char *);
#endif

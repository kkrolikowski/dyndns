#ifndef DYNSRV_H_
#define DYNSRV_H_
int bindToInterface(int portno);
int clientConn(int fd, char * cliaddr);
int readData(int fd, char * domain);
void error(char *);
#endif

#ifndef AUTH_H_
#define AUTH_H_
void get_salt(char *p, char *salt);
int userauth(char *login, char *pass);
int readAuthData(int fd, struct iovec * authdata);
#endif
#ifndef AUTH_H_
#define AUTH_H_
#include "clientmanager.h"
/*
 * get_salt -- strips salt from passwd/shadow file
 * It takes two arguments: *p which is encrypted password
 * and than saves salt in *salt string
 */
void get_salt(char *p, char *salt);
/*
 * userauth -- authenticates users
 * Returned values:
 * 0 - incorrect password
 * 1 - correct password
 */
int userauth(DB_USERDATA_t *dbdata, char *pass);

#endif

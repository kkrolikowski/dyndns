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
/*
 * isAuthorized -- check if user is authorized to update domain
 * It Takes two arguments *login and *domain.
 * Returned values:
 * -1 - user unknown or unauthorized to update domain
 *  1 - if user is authorize to update domain or create newone
 */
int isAuthorized(sqldata_t *dbdata, char *user, char *domain);
#endif

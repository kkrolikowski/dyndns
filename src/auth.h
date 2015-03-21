#ifndef AUTH_H_
#define AUTH_H_
/*
 * get_salt -- strips salt from passwd/shadow file
 * It takes two arguments: *p which is encrypted password
 * and than saves salt in *salt string
 */
void get_salt(char *p, char *salt);
/*
 * userauth -- authenticates users
 * It takes two argumets: *login and *pass
 * Returned values:
 * 0 - user authenticated
 * 1 - unknown user
 * 2 - incorrect password
 */
int userauth(char *login, char *pass);
/*
 * isAuthorized -- check if user is authorized to update domain
 * It Takes two argument *login and *domain.
 * Returned values:
 * -1 - user unknown or unauthorized to update domain
 * 1 - if user is authorize to update domain or create newone
 */
int isAuthorized(char *user, char *domain);
#endif

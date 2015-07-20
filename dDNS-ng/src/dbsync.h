struct subdomain {
	char * subd_name;
	char * ip;
	char * type;
};
struct domain {
	char * origin;
	int ttl;
	char * admin_contact;
	char * master_dns;
	int serial;
	int refresh;
	int retry;
	int expiry;
	int maximum;
	struct subdomain ** records;
};
typedef struct domain domain_t;

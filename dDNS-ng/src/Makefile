OS := $(shell uname)
ifeq ($(OS), FreeBSD)
	LIBB = -L/usr/local/lib/mysql
	HEADER = -I/usr/local/include/mysql
else
	HEADER = -I/usr/include/mysql
	LIBB = -L/usr/lib/x86_64-linux-gnu -L/usr/lib64/mysql
endif
	LIBRA = -lcrypt -lmysqlclient

all:
	gcc -o ddns-server processor.c clientmanager.c functions.c server.c common.c auth.c dbsync.c $(LIBRA) $(LIBB) $(HEADER)
	gcc -o ddns-client client.c common.c
client:
	gcc -o ddns-client client.c common.c
server:
	gcc -o ddns-server processor.c clientmanager.c functions.c server.c common.c auth.c dbsync.c $(LIBRA) $(LIBB) $(HEADER)
clean:
	rm -f ddns-client ddns-server

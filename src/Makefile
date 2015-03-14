all:
	gcc -o ddns-server ddserv.c functions.c server.c config.c auth.c -lcrypt
	gcc -o ddns-client client.c config.c
client:
	gcc -o ddns-client client.c config.c
server:
	gcc -o ddns-server ddserv.c functions.c server.c config.c auth.c -lcrypt
clean:
	rm -f ddns-client ddns-server

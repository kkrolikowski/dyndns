all:
	gcc -o ddns-server ddserv.c functions.c server.c config.c
	gcc -o ddns-client client.c config.c
client:
	gcc -o ddns-client client.c config.c
server:
	gcc -o ddns-server ddserv.c functions.c server.c config.c
clean:
	rm -f ddns-client ddns-server

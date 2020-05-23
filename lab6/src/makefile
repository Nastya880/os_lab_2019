CC =gcc
CFLAGS=-I.

all: client server

server : server.c
	$(CC) server.c -o server -lpthread -std=gnu99 $(CFLAGS)
client : client.c
	$(CC) client.c -o client -lpthread -std=gnu99 $(CFLAGS)
clean :
	rm client server core*
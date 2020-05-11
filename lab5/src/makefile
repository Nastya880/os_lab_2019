CC=gcc
CFLAGS=-I.
all : deadlock factorial with_mutex without_mutex

deadlock :
	$(CC) -o deadlock deadlock.c -lpthread $(CFLAGS)

factorial :
	$(CC) -o factorial -std=c99 factorial.c -lpthread $(CFLAGS)

without_mutex : 
	$(CC) -o without_mutex withoutmutex.c -lpthread $(CFLAGS)

with_mutex : 
	$(CC) -o with_mutex mutex.c -lpthread $(CFLAGS)

clean :
	rm deadlock factorial with_mutex without_mutex
# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=gcc
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-c -Wall


all: nomer1

nomer1: parallel_min_max.o find_min_max.o utils.o
	gcc parallel_min_max.o find_min_max.o utils.o -o nomer1

parallel_min_max.o: parallel_min_max.c
	gcc -c -Wall -std=c99 parallel_min_max.c

find_min_max.o: find_min_max.c
	gcc -c -Wall -std=c99 find_min_max.c

utils.o: utils.c
	gcc -c -Wall -std=c99 utils.c


#all: nomer1

#nomer1: parallel_min_max.o find_min_max.o utils.o
	#$(CC) parallel_min_max.o find_min_max.o utils.o -o nomer1

#parallel_min_max.o: parallel_min_max.c
#	$(CC) $(CFLAGS) parallel_min_max.c

#find_min_max.o: find_min_max.c
#	$(CC) $(CFLAGS) find_min_max.c

#utils.o: utils.c
#	$(CC) $(CFLAGS) utils.c 

clean:
	rm -rf *.o nomer1
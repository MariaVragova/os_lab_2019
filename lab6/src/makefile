CC=gcc
FLAGS=-I.

.PHONY: all clean

all : server client


server : multmodulo.o multmodulo.h 
	$(CC) server.c multmodulo.o -o server - lpthread $(FLAGS)

client : 
	$(CC) client.c multmodulo.o -o client $(FLAGS)

MultModulo.o : multmodulo.h
	$(CC) -c multmodulo.c -o multmodulo.o $(FLAGS)

clean :
	rm multmodulo.o server client ports_list.txt
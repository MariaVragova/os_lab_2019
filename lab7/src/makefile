CC=gcc
FLAGS=-I.

.PHONY: all clean

all : tcpserver tcpclient udpserver udpclient


tcpserver :  
	$(CC) tcpserver.c -o tcpserver $(FLAGS)

tcpclient : 
	$(CC) tcpclient.c -o tcpclient $(FLAGS)

udpserver :
	$(CC) udpserver.c -o udpserver $(FLAGS)

udpclient : 
	$(CC) udpclient.c -o udpclient $(FLAGS)

clean :
	rm tcpserver tcpclient udpserver udpclient
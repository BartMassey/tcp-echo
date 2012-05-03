# Copyright © 2012 Bart Massey

CC = gcc
CFLAGS = -g -Wall
ALL = echoclient echoserver

all: $(ALL)

echoclient: echoclient.c
	$(CC) $(CFLAGS) -o echoclient echoclient.c

echoserver: echoserver.c
	$(CC) $(CFLAGS) -o echoserver echoserver.c

clean:
	-rm -f $(ALL)

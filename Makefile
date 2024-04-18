# Makefile for CPE464 tcp test code
# written by Hugh Smith - April 2019

CC= gcc
CFLAGS= -g -Wall -std=gnu99
LIBS = 

OBJS = networks.o gethostbyname.o pollLib.o safeUtil.o sendReceive.o

all:   myClient myServer

myClient: myClient.c $(OBJS)
	$(CC) $(CFLAGS) -o cclient myClient.c  $(OBJS) $(LIBS)

myServer: myServer.c $(OBJS)
	$(CC) $(CFLAGS) -o server myServer.c $(OBJS) $(LIBS)

.c.o:
	gcc -c $(CFLAGS) $< -o $@ $(LIBS)

cleano:
	rm -f *.o

clean:
	rm -f server cclient *.o





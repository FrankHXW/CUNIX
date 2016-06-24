CFILES=$(wildcard *.c)
OBJS=$(CFILES:.c=.o)
CC=gcc
CFLAGS=-Wall -DDEBUG -g3 -std=c11

all: server
server: $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^
%.O: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean: $(OBJS)
	rm $(OBJS)


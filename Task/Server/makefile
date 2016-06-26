CFILES=$(wildcard *.c)
OBJS=$(CFILES:.c=.o)
CC=g++
CFLAGS=-Wall -DDEBUG -g3 -std=c++11

all: server
server: $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^
%.O: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean: $(OBJS)
	rm $(OBJS)


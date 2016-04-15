CFILES=$(wildcard *.c)
OBJS=$(CFILES:.c=.o)
CC=clang-3.5
CFLAGS=-Wall -DDEBUG -g3

all: 6
6: $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^
%.O: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean: $(OBJS)
	rm $(OBJS)


CC = gcc
CFLAGS = -g -Wall -std=c99

LIBS = -lm

DEFAULT: simplify

simplify: simplification.o pqueue.o llist.o grid.o
	$(CC) simplification.o pqueue.o llist.o grid.o -lm -o simplify

pqueue.o: pqueue.c
	$(CC) $(CFLAGS) -c $< -o $@

llist.o: llist.c
	$(CC) $(CFLAGS) -c $< -o $@

grid.o: grid.c
	$(CC) $(CFLAGS) -c $< -o $@

simplification.o: simplification.c pqueue.o llist.o grid.o
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ simplify

CC = gcc -std=c99
CFLAGS = -g -Wall -std=c99

LIBS = -lm

DEFAULT: simplify

simplify: simplification.o pqueue.o llist.o grid.o
	$(CC) $? -lm -o simplify

pqueue.o: pqueue.c
	$(CC) $(CFLAGS) -c $< -o $@

llist.o: llist.c
	$(CC) $(CFLAGS) -c $< -o $@

grid.o: grid.c
	$(CC) $(CFLAGS) -c $< -o $@

simplification.o: simplification.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ simplify

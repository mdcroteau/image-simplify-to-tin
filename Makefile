CC = gcc -std=c99
CFLAGS = -Winline -g -DNDEBUG -O3 -Wall

LIBS = -lm

DEFAULT: simplify

simplify: simplification.o pqueue.o llist.o
	$(CC) -o $@ simplification.o pqueue.o llist.o $(LIBS)

pqueue.o: pqueue.c
	$(CC) $(CFLAGS) -c pqueue.c -o pqueue.o 

llist.o: llist.c
	$(CC) $(CFLAGS) -c llist.c -o llist.o 

simplification.o: simplification.c pqueue.h llist.h
	$(CC) $(CLFAGS) -c simplification.c -o simplification.o

clean:
	rm -f *.o pqueue llist simplification

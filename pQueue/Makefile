CFLAGS = -g -Wall -l m -std=c99
CC = gcc

default: compile

compile: pQueue.o
	$(CC) $? -lm -o pQueue

pQueue.o: PriorityQueue.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ pQueue
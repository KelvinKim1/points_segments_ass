CC = gcc
CFLAGS = -Wall -std=c99

all: test

test.o: test.c points_segments.h
	$(CC) $(CFLAGS) -c test.c 

test: test.o
	$(CC) $(CFLAGS) -o test test.o 

clean:
	rm *.o test

.PHONY: all

CFLAGS=-g -pedantic -Werror -std=c99
CC=gcc

EXE = huffmanTest

all: $(EXE)

huffmanTest: heap.o huffmanTest.o huffman.o
	$(CC) $(CFLAGS) -o $@ $^

heap.o : heap.c heap.h
	$(CC) $(CFLAGS) -c $<

huffman.o : huffman.c huffman.h heap.c heap.h
	$(CC) $(CFLAGS) -c $<

huffmanTest.o : huffmanTest.c huffman.h heap.c heap.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o huffmanTest


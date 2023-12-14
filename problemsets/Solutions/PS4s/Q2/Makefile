.PHONY: all

CFLAGS=-g -lm -pedantic -Werror -std=c99
CC=gcc

EXE = countBitsTest compressTest

all: $(EXE)

countBitsTest: countBitsTest.o compression.o file.o huffman.o heap.o
	$(CC) $(CFLAGS) -o $@ $^

compressTest: compressTest.o compression.o file.o huffman.o heap.o
	$(CC)  $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o countBitsTest compressTest


.PHONY: all

CFLAGS=-g -pedantic -Werror -std=c99
CC=gcc

EXE = compress

all: $(EXE)

compress: heap.o file.o huffman.o compression.o compress.o
	$(CC)  $(CFLAGS)  -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o compress


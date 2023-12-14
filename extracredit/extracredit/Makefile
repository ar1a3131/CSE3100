CC=cc
CFLAGS=-O3

all: rpair

rpair: rpair.o
	$(CC) $(CFLAGS) -o $@ $^ -lrt

%.o : %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -rf *.o rpair *~

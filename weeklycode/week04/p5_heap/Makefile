CFLAGS = -g
CC = cc
OFILES = heap.o heapTest.o

all: heapTest

heapTest: $(OFILES)
	$(CC) $(OFILES) -o $@

%.o: %.c heap.h
	$(CC) $(CFLAGS) -c $<
    
clean:
	rm -rf *.o *~ heapTest	    

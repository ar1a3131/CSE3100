CFLAGS = -g
CC = cc
OFILES = stack.o stacktest.o

all: stacktest

stacktest: $(OFILES)
	$(CC) $(OFILES) -o $@

stack.o: stack.c stack.h
	$(CC) -c $(CFLAGS) $<

stacktest.o: stacktest.c stack.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -rf *.o *~ stacktest	

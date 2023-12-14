all: uftest

uftest: list.o ufind.o uftest.o
	cc -g -o $@ $^

%.o : %.c
	cc -c $<

clean:
	rm -rf *.o uftest

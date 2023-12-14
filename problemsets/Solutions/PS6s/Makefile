all: Q2server Q2client Q2service

Q2server : Q2server.c
	$(CC) $(CFLAGS) -o $@ $?

Q2client : Q2client.c
	$(CC) $(CFLAGS) -o $@ $?

Q2service : Q2service.c
	$(CC) $(CFLAGS) -o $@ $?

clean:
	rm -rf *.o Q2client Q2server Q2service *~

CLI = client.o
SRV = server.o
OBJS = 
CFLAGS = -g
CC = cc

all: client server

client: $(CLI) $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o client $(CLI) $(OBJS)

server: $(SRV) $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o server $(SRV) $(OBJS)

clean: 
	rm -rf *.o client server *~

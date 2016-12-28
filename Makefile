all: server client

server: server.o
	gcc server.o -o server -lpthread

client: client.o
	gcc client.o -o client -lpthread

clean:
	rm *.o

all: server client other clean

server: server.o function.o utility.o
	gcc -Wall server.o function.o utility.o -o server 

client: client.o function.o utility.o
	gcc -Wall client.o function.o utility.o -o client

other: client.o function.o utility.o
	gcc -Wall client.o function.o utility.o -o other

client.o: client.c function.h utility.h
	gcc -Wall -g -c -std=c89 client.c -o client.o

server.o: server.c function.h utility.h 
	gcc -Wall -g -c -std=c89 server.c -o server.o

utility.o: utility.c utility.h structure.h function.h
	gcc -Wall -g -c -std=c89 utility.c -o utility.o

function.o: function.c function.h structure.h
	gcc -Wall -g -c -std=c89 function.c -o function.o

.PHONY: clean

clean: 
	rm *.o
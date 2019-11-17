client : client.c libmult.so
	gcc client.c -lpthread -L. -lmult -o client

server : server.c libmult.so
	gcc server.c -lpthread -L. -lmult -o server

libmult.so : libmult.c
	gcc -c libmult.c -fPIC -o libmult.o
	gcc -shared libmult.o -o libmult.so
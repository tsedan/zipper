CC = gcc
CFLAGS = -O3 -std=c11 -Wall -pedantic

all: bin/server bin/client

bin/server: bin/server.o
	$(CC) -g -o bin/server bin/server.o $(CFLAGS) -lpthread

bin/server.o: src/server.c src/shared.h
	mkdir -p bin
	$(CC) -g -c -o bin/server.o src/server.c $(CFLAGS)

bin/client: bin/client.o bin/game.o bin/input.o
	$(CC) -g -o bin/client bin/client.o bin/game.o bin/input.o $(CFLAGS) -lncurses

bin/client.o: src/client.c src/client.h src/shared.h
	mkdir -p bin
	$(CC) -g -c -o bin/client.o src/client.c $(CFLAGS)

bin/game.o: src/game.c src/client.h src/shared.h
	mkdir -p bin
	$(CC) -g -c -o bin/game.o src/game.c $(CFLAGS)

bin/input.o: src/input.c src/client.h src/shared.h
	mkdir -p bin
	$(CC) -g -c -o bin/input.o src/input.c $(CFLAGS)

clean:
	rm -r bin

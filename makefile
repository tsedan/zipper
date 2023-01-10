CFLAGS = -O3 -std=c11 -Wall -pedantic

all: bin/server bin/main

bin/server: bin/server.o
	clang -g -o bin/server bin/server.o $(CFLAGS)

bin/server.o: svr/server.c
	mkdir -p bin
	clang -g -c -o bin/server.o svr/server.c $(CFLAGS)

bin/main: bin/main.o bin/game.o
	clang -g -o bin/main bin/main.o bin/game.o $(CFLAGS) -lncurses

bin/main.o: src/main.c src/main.h
	mkdir -p bin	
	clang -g -c -o bin/main.o src/main.c $(CFLAGS)

bin/game.o: src/game.c src/game.h src/main.h
	mkdir -p bin	
	clang -g -c -o bin/game.o src/game.c $(CFLAGS)

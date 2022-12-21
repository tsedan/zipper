bin/main: bin/main.o
	clang -g -o bin/main bin/main.o -std=c14 -Wall -pedantic -lncurses

bin/main.o: src/main.c
	mkdir -p bin
	clang -g -c -o bin/main.o src/main.c

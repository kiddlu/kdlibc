all: main

main: main.o add.o
	g++ -std=c99 -o $@ $^

main.o: main.cpp
	g++ -c $^ -o $@

add.o: add.c
	gcc -c $^ -o $@

clean:
	rm main *.o

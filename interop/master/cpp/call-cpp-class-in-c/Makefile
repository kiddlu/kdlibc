all: main

main: main.o add.o
	gcc -o $@ $^

main.o: main.c
	gcc -c $^ -o $@

add.o: add.cpp
	g++ -c $^ -o $@

clean:
	rm main *.o *.a


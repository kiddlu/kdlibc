all: main

main: main.o libadd.a
	g++ -std=c99 -o $@ $^

main.o: main.cpp
	g++ -c $^ -o $@

libadd.a: add.o
	ar -r $@ $^

add.o: add.c
	gcc -c $^ -o $@

clean:
	rm main *.o *.a

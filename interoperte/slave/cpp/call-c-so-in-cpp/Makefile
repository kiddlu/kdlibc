all: main

main: main.o libcat.so
	g++ -Wall -Wl,-rpath,. -o $@ $^

main.o: main.cpp
	g++ -c $^ -o $@

libcat.so: cat.o
	gcc -shared -Wl,-soname,$@ -o $@ $^

cat.o: cat.c
	gcc -fPIC -c $^ -o $@

clean:
	rm main *.o *.so

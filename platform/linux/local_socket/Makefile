all: main

main: main.o libcat.so
	gcc -Wall -Wl,-rpath,. -o $@ $^ -ldl

main.o: main.c
	g++ -c $^ -o $@

libcat.so: cat.o
	gcc -shared -Wl,-soname,$@ -o $@ $^

cat.o: cat.c
	gcc -fPIC -c $^ -o $@

clean:
	rm main *.o *.so

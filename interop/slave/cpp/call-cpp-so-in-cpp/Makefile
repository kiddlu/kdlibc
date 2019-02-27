all: main

main: main.o libcat.so
	g++ -Wall -Wl,-rpath,. -o $@ $^

main.o: main.cpp
	g++ -c $^ -o $@

libcat.so: cat.o
	g++ -shared -Wl,-soname,$@ -o $@ $^

cat.o: cat.cpp
	g++ -fPIC -c $^ -o $@

clean:
	rm main *.o *.so

all: main

main: main.o libstatic.a libshared.so
	gcc -Wl,-rpath,. -o $@ $^

main.o: main.c
	gcc -c $^ -o $@

libstatic.a: static.o
	ar -r $@ $^

static.o: static.c
	gcc -c $^ -o $@

libshared.so: shared.o
	gcc -shared -Wl,-soname,$@ -o $@ $^

shared.o: shared.c
	gcc -fPIC -c $^ -o $@

clean:
	rm main *.o *.a *.so


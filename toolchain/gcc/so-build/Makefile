all: main

main: main.o libtest.so
	gcc -Wl,-rpath,. -o $@ $^

main.o: main.c
	gcc -c $^ -o $@

libtest.so: test.o
	gcc -shared -Wl,-soname,$@ -o $@ $^

test.o: test.c
	gcc -fPIC -c $^ -o $@

clean:
	rm main *.o *.so


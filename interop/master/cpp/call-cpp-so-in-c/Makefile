all: main

main: main.o libadd.so libcwrapper.so
	gcc -Wl,-rpath,. -o $@ $^ -L. -lcwrapper -ladd

main.o: main.c
	gcc -c $^ -o $@

libadd.so: add.o
	g++ -shared -Wl,-soname,$@ -o $@ $^
add.o: add.cpp
	g++ -fPIC -c $^ -o $@

libcwrapper.so: cwrapper.o
	g++ -shared -Wl,-soname,$@ -o $@ $^
cwrapper.o: cwrapper.cpp
	g++ -fPIC -c $^ -o $@

clean:
	rm main *.o *.so


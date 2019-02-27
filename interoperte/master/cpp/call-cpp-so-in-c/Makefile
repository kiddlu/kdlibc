all: main

main: main.o libadd.a libcwrapper.a
	gcc -o $@ $^ -L. -lcwrapper -ladd

main.o: main.c
	gcc -c $^ -o $@

libadd.a: add.o
	ar -r $@ $^
add.o: add.cpp
	g++ -fPIC -c $^ -o $@

libcwrapper.a: cwrapper.o
	ar -r $@ $^
cwrapper.o: cwrapper.cpp
	g++ -fPIC -c $^ -o $@

clean:
	rm main *.o *.a


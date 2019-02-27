all: main

main: libpycallclass.so
	python pycallclass.py

libpycallclass.so: pycallclass.cpp
	g++ -fPIC -shared -Wl,-soname,$@ -o $@ $^

clean:
	rm *.so


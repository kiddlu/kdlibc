all: main

main: test
	python main.py

test: main.cpp
	g++ -o $@ $^

clean:
	rm test

all: main

main: ifelse.i
	gcc -std=c99 -o $@ $^

ifelse.i: ifelse.c
	gcc -std=c99 -E -o $@ $^

clean:
	rm main ifelse.i

all: logind

logind: logind.c
	gcc -std=c99 -o $@ $^ -lutil

clean:
	rm logind

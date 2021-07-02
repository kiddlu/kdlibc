all: logind cmdd

logind: logind.c
	gcc -std=c99 -o $@ $^ -lutil

cmdd: cmdd.c
	gcc -std=c99 -o $@ $^

clean:
	rm logind cmdd

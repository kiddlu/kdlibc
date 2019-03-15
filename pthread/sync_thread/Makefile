all: main

main: sync_thread.c
	gcc -std=c99 -o $@ $^ -lpthread

clean:
	rm main

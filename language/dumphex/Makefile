all: encode

encode: encode.c dumphex.c
	gcc -std=c99 -o $@ $^

clean:
	rm encode

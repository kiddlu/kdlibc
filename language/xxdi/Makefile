all: encode

encode: encode.c xxdi.c
	gcc -std=c99 -o $@ $^

## ./encode file > decode.h

decode: decode.c
	gcc -std=c99 -o $@ $^

clean:
	rm encode decode

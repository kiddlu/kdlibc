all: write read

write: write.c 
	gcc -std=c99 -o $@ $^

read: read.c
	gcc -std=c99 -o $@ $^

clean:
	rm write read counter

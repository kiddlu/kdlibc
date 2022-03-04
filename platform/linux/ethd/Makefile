all: ethc ethd

ethc: ethc.c
	gcc -O2 -o $@ $^ -lpthread

ethd: ethd.c
	gcc -O2 -o $@ $^ -lpthread

clean:
	rm ethc ethd

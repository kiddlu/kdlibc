all: bsdiff bspatch

bsdiff: bsdiff.c
	gcc -O2 -DBSDIFF_EXECUTABLE -o $@ $^ -lbz2

bspatch: bspatch.c
	gcc -O2 -DBSPATCH_EXECUTABLE -o $@ $^ -lbz2

clean:
	rm bsdiff bspatch *.o out.*

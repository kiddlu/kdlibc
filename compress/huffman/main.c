#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "huffman.h"

#define TEST_MAXLEN 1048576
#define TEST_ITER 1024

static unsigned char testin[TEST_MAXLEN];
static unsigned char testout[TEST_MAXLEN * 2];
static unsigned char testver[TEST_MAXLEN];
//static unsigned char huffbuf[HUFFHEAP_SIZE];

int main(int argc,char **argv)
{
        unsigned long i,k,l,cl,dcl;
        int v;
        unsigned char mask;

        srand(time(0));

        for(k=0;k<TEST_ITER;++k) {
                l = (rand() % TEST_MAXLEN) + 1;
                mask = (rand() & 0xff);
                for(i=0;i<l;++i)
                        testin[i] = (unsigned char)(rand() & 0xff) & mask;
                cl = huffman_compress(testin,l,testout,sizeof(testout), NULL);
                if (cl) {
                        memset(testver,0,sizeof(testver));
                        dcl = huffman_decompress(testout,cl,testver,sizeof(testver), NULL);
                        v = ((dcl)&&(!memcmp(testver,testin,l)));
                        printf("[%d] in: %d, out: %d, verified: %s\n",(int)k,(int)l,(int)cl,(v) ? "OK" : "FAIL");
                } else printf("[%d] in: %d, out: FAIL\n",(int)k,(int)l);
        }
/*
        printf("\nFuzzing decompress function...\n");
        for(;;) {
                l = (rand() % TEST_MAXLEN) + 1;
                mask = (rand() & 0xff);
                for(i=0;i<l;++i)
                        testin[i] = (unsigned char)(rand() & 0xff) & mask;
                huffman_decompress(testin,l,testver,sizeof(testver), NULL);
                printf("."); fflush(stdout);
        }
*/
        return 0;
}

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct type {
	int age;
	float height;
	char name[8];
};

void memdump(void *addr, uint32_t size)
{
    int index;
    
    while (size > 0) {
        fprintf(stdout, "%p: ", addr);
        index = 0;

        // 32 bytes in the line
        while (index < 32) {
            fprintf(stdout, "0x%02x ", *((unsigned char *)addr));
            
            index++;
            addr++;
            size--;
 
            if (size <= 0)
                break;
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
    return;
}

char bob_buf[]={0x14,0x00,0x00,0x00,0xcd,0x4c,0x30,0x43,0x62,0x6f,0x62,0x00,0x00,0x00,0x00,0x00};

struct xxx {
	char a;
	char b;
	char c;
	char d;
};

int main()
{
/*
	struct type mike, bob;
	struct type *p;

	mike.age = 18;
	mike.height = 188.6;
	strcpy(mike.name, "mike");

	bob.age = 20;
	bob.height = 176.3;
	strcpy(bob.name, "bob");


	memdump(&mike, sizeof(struct type));
	memdump(&bob, sizeof(struct type));
	
	p = (struct type *)bob_buf;
	memdump(p, sizeof(struct type));
	printf("buf height is %f\n", p->height);
*/
	char hbuf[]={0xcd,0x4c,0x30,0x43};
//	float *p = (float *)hbuf;
//	float v = *p;
	printf("buf height is %f\n", *((float *)hbuf));
	memdump(hbuf, 4);
	
	struct xxx nbuf;
	nbuf.a = 0xcd;
	nbuf.b = 0x4c;
	nbuf.c = 0x30;
	nbuf.d = 0x43;
	memdump(&nbuf, sizeof(struct xxx));
	printf("buf height is %f\n", *((float *)&nbuf));
	
	float nf = *((float *)&nbuf);
	printf("buf height is %f\n", nf);
	
}

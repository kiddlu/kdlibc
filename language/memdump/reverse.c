#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

uint8_t str[]={

};


void reverse()
{
    int i= 0;
    FILE *fp = fopen("./restore", "wb+");

    for(i=0;i<sizeof(str);i++)
        fwrite(&str[i], 1, 1,fp);

    fclose(fp);
}

int main()
{
    reverse();
}

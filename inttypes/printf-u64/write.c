#include <stdint.h>
#include <stdio.h>

int main()
{
    uint64_t counter = -1;

    FILE *fp = fopen("./counter", "wb+"); 
    fwrite(&counter, sizeof(counter), 1, fp);
    fclose(fp);
}

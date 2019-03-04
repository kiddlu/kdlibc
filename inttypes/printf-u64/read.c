#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

int main()
{
    uint64_t counter;

    FILE *fp = fopen("./counter", "rb"); 
    fread(&counter, sizeof(counter), 1, fp);
    fclose(fp);

    printf("counter is %"PRIu64"", counter);
}

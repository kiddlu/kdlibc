#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

int main()
{
    uint64_t uint;
    int64_t sint;

    FILE *fp = fopen("./uint", "rb"); 
    fread(&uint, sizeof(uint), 1, fp);
    fclose(fp);

    fp = fopen("./sint", "rb"); 
    fread(&sint, sizeof(sint), 1, fp);
    fclose(fp);

    printf("uint is %"PRIu64"\n", uint);
    printf("sint is %"PRId64"\n", sint);
}

#include <stdint.h>
#include <stdio.h>

int main()
{
    uint64_t uint = -1;
    int64_t sint = -1;

    FILE *fp = fopen("./uint", "wb+"); 
    fwrite(&uint, sizeof(uint), 1, fp);
    fclose(fp);

    fp = fopen("./sint", "wb+"); 
    fwrite(&sint, sizeof(sint), 1, fp);
    fclose(fp);
}

#include <stdio.h>

#include "decode.h"

void usage(void)
{
        printf("useage: file\n");
}


int main(int argc, char **argv)
{
    if(argc != 2) {
        usage();
        return -1;
    }

    FILE *fp = fopen(argv[1], "wb+");
    fwrite(hex_string, hex_string_len, 1, fp);
    fclose(fp);
}

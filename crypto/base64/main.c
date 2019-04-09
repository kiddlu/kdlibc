#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "base64.h"

//use xxd -p -r str to recover buf to file
void print_hex(unsigned char str[], unsigned int len)
{
    int idx;

    for(idx = 0; idx < len; idx++)
        printf("%02x", str[idx]);
}

size_t get_file_size(FILE *fp)
{
    size_t length;

    fseek(fp, 0L, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    return length;
}

void usage(void)
{
    printf("useage: file\n");
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        usage();
        return -1;
    } else if ( 0 != access(argv[1], F_OK)) {
        usage();
        return -1;
    }

    FILE *fp = fopen(argv[1], "rb");
    size_t len = get_file_size(fp);
    size_t olen;
    unsigned char *out_buf;

    char *data = malloc(len);
    fread(data, len, 1, fp);
#if 0
    base64_encode(data, len, NULL, &olen, 1);
    out_buf = malloc(olen);
    base64_encode(data, len, out_buf, &olen, 1);
    printf("%s\n", out_buf);
#else
    olen = base64_decode(data, NULL, len);
    out_buf = malloc(olen);
    printf("outlen is %d\n", olen);
    olen = base64_decode(data, out_buf, len);
    print_hex(out_buf, olen);
#endif

    return 0;
}

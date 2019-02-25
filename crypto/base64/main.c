//copy from https://github.com/B-Con/crypto-algorithms/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>

typedef unsigned char BYTE;             // 8-bit byte

// Returns the size of the output. If called with out = NULL, will just return
// the size of what the output would have been (without a terminating NULL).
void base64_encode(const BYTE in[], size_t len, BYTE out[], size_t *olen, int newline_flag);

size_t base64_decode(const BYTE in[], BYTE out[], size_t len);

#define NEWLINE_INVL 76

// Note: To change the charset to a URL encoding, replace the '+' and '/' with '*' and '-'
static const BYTE charset[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

BYTE revchar(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
        ch -= 'A';
    else if (ch >= 'a' && ch <='z')
        ch = ch - 'a' + 26;
    else if (ch >= '0' && ch <='9')
        ch = ch - '0' + 52;
    else if (ch == '+')
        ch = 62;
    else if (ch == '/')
        ch = 63;

    return(ch);
}

void base64_encode(const BYTE in[], size_t len, BYTE out[], size_t *olen, int newline_flag)
{
    size_t idx, idx2, blks, blk_ceiling, left_over, newline_count = 0;

    blks = (len / 3);
    left_over = len % 3;

    if (out == NULL) {
        idx2 = blks * 4 ;
        if (left_over)
            idx2 += 4;
        if (newline_flag)
            idx2 += len / 57;   // (NEWLINE_INVL / 4) * 3 = 57. One newline per 57 input bytes.
    }
    else {
        // Since 3 input bytes = 4 output bytes, determine out how many even sets of
        // 3 bytes the input has.
        blk_ceiling = blks * 3;
        for (idx = 0, idx2 = 0; idx < blk_ceiling; idx += 3, idx2 += 4) {
            out[idx2]     = charset[in[idx] >> 2];
            out[idx2 + 1] = charset[((in[idx] & 0x03) << 4) | (in[idx + 1] >> 4)];
            out[idx2 + 2] = charset[((in[idx + 1] & 0x0f) << 2) | (in[idx + 2] >> 6)];
            out[idx2 + 3] = charset[in[idx + 2] & 0x3F];
            // The offical standard requires a newline every 76 characters.
            // (Eg, first newline is character 77 of the output.)
            if (((idx2 - newline_count + 4) % NEWLINE_INVL == 0) && newline_flag) {
                out[idx2 + 4] = '\n';
                idx2++;
                newline_count++;
            }
        }

        if (left_over == 1) {
            out[idx2]     = charset[in[idx] >> 2];
            out[idx2 + 1] = charset[(in[idx] & 0x03) << 4];
            out[idx2 + 2] = '=';
            out[idx2 + 3] = '=';
            idx2 += 4;
        }
        else if (left_over == 2) {
            out[idx2]     = charset[in[idx] >> 2];
            out[idx2 + 1] = charset[((in[idx] & 0x03) << 4) | (in[idx + 1] >> 4)];
            out[idx2 + 2] = charset[(in[idx + 1] & 0x0F) << 2];
            out[idx2 + 3] = '=';
            idx2 += 4;
        }
    }

    (*olen) = idx2;
    return;
}

size_t base64_decode(const BYTE in[], BYTE out[], size_t len)
{
    BYTE ch;
    size_t idx, idx2, blks, blk_ceiling, left_over;

    if (in[len - 1] == '=')
        len--;
    if (in[len - 1] == '=')
        len--;

    blks = len / 4;
    left_over = len % 4;

    if (out == NULL) {
        if (len >= 77 && in[NEWLINE_INVL] == '\n')   // Verify that newlines where used.
            len -= len / (NEWLINE_INVL + 1);
        blks = len / 4;
        left_over = len % 4;

        idx = blks * 3;
        if (left_over == 2)
            idx ++;
        else if (left_over == 3)
            idx += 2;
    }
    else {
        blk_ceiling = blks * 4;
        for (idx = 0, idx2 = 0; idx2 < blk_ceiling; idx += 3, idx2 += 4) {
            if (in[idx2] == '\n')
                idx2++;
            out[idx]     = (revchar(in[idx2]) << 2) | ((revchar(in[idx2 + 1]) & 0x30) >> 4);
            out[idx + 1] = (revchar(in[idx2 + 1]) << 4) | (revchar(in[idx2 + 2]) >> 2);
            out[idx + 2] = (revchar(in[idx2 + 2]) << 6) | revchar(in[idx2 + 3]);
        }

        if (left_over == 2) {
            out[idx]     = (revchar(in[idx2]) << 2) | ((revchar(in[idx2 + 1]) & 0x30) >> 4);
            idx++;
        }
        else if (left_over == 3) {
            out[idx]     = (revchar(in[idx2]) << 2) | ((revchar(in[idx2 + 1]) & 0x30) >> 4);
            out[idx + 1] = (revchar(in[idx2 + 1]) << 4) | (revchar(in[idx2 + 2]) >> 2);
            idx += 2;
        }
    }

    return(idx);
}

void print_hex(unsigned char str[], unsigned int len)
{
    int idx;

    for(idx = 0; idx < len; idx++)
        printf("%02x", str[idx]);
}

unsigned int get_file_size(FILE *fp)
{
   unsigned int length;

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
    uint32_t len = get_file_size(fp);
    uint32_t olen;
    unsigned char *out_buf;

    char *data = malloc(len);
    fread(data, len, 1, fp);

    base64_encode(data, len, NULL, &olen, 1);
    out_buf = malloc(olen);
    base64_encode(data, len, out_buf, &olen, 1);

    printf("%s\n", out_buf);
    return 0;
}


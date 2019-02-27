#ifndef __BASE64_H__
#define __BASE64_H__
typedef unsigned char BYTE;             // 8-bit byte

// Returns the size of the output. If called with out = NULL, will just return
// the size of what the output would have been (without a terminating NULL).
void base64_encode(const BYTE in[], size_t len, BYTE out[], size_t *olen, int newline_flag);

size_t base64_decode(const BYTE in[], BYTE out[], size_t len);

#define NEWLINE_INVL 76
#endif

#include <stddef.h>

//convert char to hex
int char2hex(char c)
{
	if ((c >= 'A') && (c <= 'Z'))
	{
		return c - 'A' + 10;
	}
	else if ((c >= 'a') && (c <= 'z'))
	{
		return c - 'a' + 10;
	}
	else if ((c >= '0') && (c <= '9'))
	{
		return c - '0';
	}
	return -1;
}

//convert hex to char
char hex2char(int h)
{
    const char h2c_table[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    if((h < 0) || (h > 15))
        return '\0';
    else
        return h2c_table[h];
}

//convert hex to str
int hex2str(unsigned char *hex, int hex_len, char *str, int str_len)
{
    unsigned char high;
    unsigned char low;

    unsigned char character_case = 'a';

    if (str_len < hex_len * 2)
        return -1;

    for (int i = 0; i < hex_len; i++) {
        high = (hex[i] & 0xf0) >> 4;

        if (high <= 9)
            high += 0x30;
        else if (high >= 10 && high <= 15)
            high = high - 10 + character_case;
        else
            return -1;

        low = hex[i] & 0x0f;
        if (low <= 9)
            low += 0x30;
        else if (low >= 10 && low <= 15)
            low = low - 10 + character_case;
        else
            return -1;

        str[i * 2] = high;
        str[i * 2 + 1] = low;
    }

    return 0;
}

//convert str to hex
int str2hex(char *str, int str_len, unsigned char *hex, int hex_len)
{
    unsigned char high, low;
    unsigned int real_str_len = strlen(str);

    if(str_len > real_str_len)
        return -1;

    if(hex_len * 2 < str_len)
        return -1;

    for (int i = 0, j = 0; i < len; i++) {

        high = str[i];
        if (high >= 'A' && high <= 'F')
            high = high - 'A' + 10;
        else if (high >= 'a' && high <= 'f')
            high = high - 'a' + 10;
        else if (high >= '0' && high <= '9')
            high -= 0x30;
        else
            return -1;

        low = str[++i];
        if (low >= 'A' && low <= 'F')
            low = low - 'A' + 10;
        else if (low >= 'a' && low <= 'f')
            low = low - 'a' + 10;
        else if (low >= '0' && low <= '9')
            low -= 0x30;
        else
            return -1;

        hex[j++] = (high << 4) | low;
    }

    return 0;
}


//convert unsigned int to a string
char *utoa(unsigned int value, char *str, int base)
{
  const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  int i, j;
  unsigned int remainder;
  char c;

  /* not support */
  if((base < 2) || (base > 36)) {
    str[0] = '\0';
    return NULL;
  }

  /* Convert to string. Digits are in reverse order. */
  i = 0;
  do {
    remainder = value % base;
    str[i++] = digits[remainder];
    value = value / base;
  } while (value != 0);

  str[i] = '\0';

  /* Reverse string */
  for (j = 0, i--; j < i; j++, i--) {
    c = str[j];
    str[j] = str[i];
    str[i] = c;
  }

  return str;
}

//convert int to a string: http://www.strudel.org.uk/itoa/
char* itoa(int value, char* result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) { *result = '\0'; return result; }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );

  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

//convert a string to int
int atoi(const char *str)
{

}

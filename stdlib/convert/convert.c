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
        return NULL;
    else
        return h2c_table[h];
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

//int float or other types to str
#include <stdio.h>

#define M_PI             3.14159265358979323846

int main()
{
   char str[80];

   sprintf(str, "Pi = %f", M_PI);
   puts(str);
   
   return(0);
}

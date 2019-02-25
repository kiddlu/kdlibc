#include <stdio.h> 
  
#define MACRO(num, str) ({\
            printf("%d", num);\
            printf(" is");\
            printf(" %s number", str);\
            printf("\n");\
           })

#define MACRO_DO(num, str) do {\
            if((num) == 2){\
                printf("hahaha ");\
            }\
            printf("%d", num);\
            printf(" is");\
            printf(" %s number", str);\
            printf("\n");\
           } while(0)

int main(void)
{ 
    int num;
  
    printf("Enter a number: ");
    scanf("%d", &num);
  
    if (num & 1)
        MACRO(num, "Odd");
    else
        MACRO_DO(num, "Even");
  
    return 0;
} 

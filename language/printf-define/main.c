#include <stdio.h>

#define DEBUG

#ifdef DEBUG  
#define LOG(format, ...) fprintf(stdout, format, ##__VA_ARGS__)  
#else  
#define LOG(format, ...)  
#endif  

int main()
{
    LOG("Hello World\n");
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char buff[] = "The Legend of Zelda: Breath of the Wild";  
  
int main(int argc, char **argv)  
{  
    int len = strlen(buff);
    FILE *fd = fmemopen(buff, len, "r");
    if(fd == NULL)
    {
        printf("get file error!\n");
        return -1;
    }

    char ch;
    while((ch = fgetc(fd)) != EOF)
        printf("%c", ch);
    puts("");

    fclose(fd);

    return 0;  
}


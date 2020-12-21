#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_FILE   "/tmp/local_sock"

int main()
{
    int sockfd = socket(AF_LOCAL,SOCK_DGRAM,0);
    if(0 > sockfd)
    {
        perror("socket");
        return -1;
    }


    struct sockaddr_un addr = {};
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path,SOCK_FILE);


    if(connect(sockfd,(struct sockaddr*)&addr,sizeof(addr)))
    {
        perror("connect");
        return -1;
    }


    char buf[1024] = {};
    for(;;)
    {
        printf(">");
        gets(buf);
        int ret = write(sockfd,buf,strlen(buf)+1);
        if(0 > ret)
        {
            perror("write");
            return -1;
        }
        printf("read:%s\n",buf);
        if(0 == strcmp("quit",buf))
        {
            printf("quit！\n");
            break;
        }
    }

    close(sockfd);
}

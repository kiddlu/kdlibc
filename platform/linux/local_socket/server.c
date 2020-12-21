#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_FILE   "/tmp/local_sock"

int main()
{
    //创建socket
    int sockfd = socket(AF_LOCAL,SOCK_DGRAM,0);
    if(0 > sockfd)
    {
        perror("socket");
        return -1;
    }


    struct sockaddr_un addr = {};
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path,SOCK_FILE);


    if(bind(sockfd,(struct sockaddr*)&addr,sizeof(addr)))
    {
        perror("bind");
        return -1;
    }
	
    char buf[1024] = {};
    for(;;)
    {
        int ret = read(sockfd,buf,sizeof(buf));
        if(0 > ret)
        {
            perror("read");
            return -1;
        }
        printf("read:%s\n",buf);
        if(0 == strcmp("quit",buf))
        {
            printf("server quit\n");
            break;
        }
    }
	

    close(sockfd);
    unlink(SOCK_FILE);
}
#include "head.h"

int tcpInit(const char *ip,const char *port, int *psockfd)
{
    // socket
    *psockfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(*psockfd,-1,"socket");
    // bind
    struct sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port));
    int ret = bind(*psockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"bind");
    // listen
    listen(*psockfd,50);

    return 0;
}


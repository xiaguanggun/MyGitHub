#include <54func.h>

int main(){
    const char query[] ="GET / HTTP/1.0\r\n"
        "Host: www.baidu.com\r\n"
        "\r\n";
    const char hostname[] = "www.baidu.com";
    // 续写代码 
    // 获取IP
    struct hostent *entry = gethostbyname(hostname);
    if(entry == NULL)
    {
        herror("gethostbyname");
        exit(-1);
    }

    //发起连接
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket");

    struct sockaddr_in serverAddr;
    memcpy(&serverAddr.sin_addr,entry->h_addr_list[0],sizeof(serverAddr.sin_addr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(80);
    int ret = connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"connect");

    ssize_t sret = send(sockfd,query,sizeof(query),0);

    char buf[4096] = {0};
    while(1)
    {
        bzero(buf,sizeof(buf));
        sret = recv(sockfd,buf,sizeof(buf),0);
        if(sret == 0)
        {
            break;
        }
        printf("%s",buf);
    }
    printf("\n");

    return 0;
}

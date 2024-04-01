#include <54func.h>

int main(int argc, char * argv[])
{
    // ./hw6_client 192.168.244.129 54321
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket");

    struct sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    int ret = connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"connect");

    fd_set monitorSet,readySet; //监听集合，就序集合
    char buf[4096] = {0};
    FD_ZERO(&monitorSet);
    FD_SET(sockfd,&monitorSet);
    FD_SET(STDIN_FILENO,&monitorSet);
    while(1)
    {
        //初始化就序集合(监听用)
        memcpy(&readySet,&monitorSet,sizeof(monitorSet));
        int ret = select(sockfd+1,&readySet,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
        //服务器断开连接&接收消息
        if(FD_ISSET(sockfd,&readySet))
        {
            bzero(buf,sizeof(buf));
            ssize_t sret = recv(sockfd,buf,sizeof(buf),0);
            //服务器断开连接
            if(sret == 0)
            {
                close(sockfd);
                printf("服务器断开连接！\n");
                exit(0);
            }
            //接收消息
            printf("%s\n",buf);
        }
        //主动断开连接&发消息
        if(FD_ISSET(STDIN_FILENO,&readySet))
        {
            ssize_t sret = read(STDIN_FILENO,buf,sizeof(buf));
            if(sret == 0)
            {
                close(sockfd);
                exit(0);
            }
            send(sockfd,buf,sret,0);
        }
    }

    return 0;
}


#include <54func.h>

int main(int argc, char * argv[])
{
    // ./hw3_select_C 192.168.244.129 12345
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sockfd,-1,"socket");

    char buf[4096] = {0};

    struct sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    //建立连接
    ssize_t sret = sendto(sockfd,"aqiang connected!",18,0,
                          (struct sockaddr *)&serverAddr,
                          sizeof(serverAddr));
    ERROR_CHECK(sret,-1,"sendto");
    //设置监听集合
    fd_set monitorSet,readySet; //监听集合，就序集合
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
            ssize_t sret = recvfrom(sockfd,buf,sizeof(buf),0,
                                    NULL,NULL);
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
                sret = sendto(sockfd,buf,0,0,
                              (struct sockaddr *)&serverAddr,
                              sizeof(serverAddr));
                ERROR_CHECK(sret,-1,"sendto");
                close(sockfd);
                exit(0);
            }
            sret = sendto(sockfd,buf,sret,0,
                          (struct sockaddr *)&serverAddr,
                          sizeof(serverAddr));
            ERROR_CHECK(sret,-1,"sendto");
        }
    }

    return 0;
}


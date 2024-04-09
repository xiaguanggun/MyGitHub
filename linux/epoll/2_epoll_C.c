#include <54func.h>

int main(int argc, char * argv[])
{
    // ./hw4_epoll_C 192.168.244.129 12345
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket");

    struct sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    int ret = connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"connect");

    //创建epoll文件对象
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd,-1,"epoll_create");

    //初始化监听集合stdin+sockfd,通信用
    struct epoll_event events;
    events.events = EPOLLIN;
    events.data.fd = sockfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&events);
    events.events = EPOLLIN;
    events.data.fd = STDIN_FILENO;
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&events);

    char buf[4096] = {0};
    while(1)
    {
        struct epoll_event readySet[2];
        int readyNum = epoll_wait(epfd,readySet,sizeof(readySet),-1);
        ERROR_CHECK(readyNum,-1,"epoll_wait");

        //遍历就序集合数组
        for(int i = 0;i < readyNum;++i)
        {
            if(readySet[i].data.fd == sockfd)
            {
                //服务器断开连接&接收消息
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
            else if(readySet[i].data.fd == STDIN_FILENO)
            {
                //主动断开连接&发消息
                bzero(buf,sizeof(buf));
                ssize_t sret = read(STDIN_FILENO,buf,sizeof(buf));
                if(sret == 0)
                {
                    close(sockfd);
                    exit(0);
                }
                sret = send(sockfd,buf,sret,0);
                ERROR_CHECK(sret,-1,"send");
            }
        }
    }
    return 0;
}


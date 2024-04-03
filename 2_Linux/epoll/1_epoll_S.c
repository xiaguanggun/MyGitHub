#include <54func.h>

int main(int argc, char * argv[])
{
    // ./hw2_epoll_S 192.168.244.129 12345
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket");

    //允许重用
    int reuse = 1;
    int ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR
                         ,&reuse,sizeof(reuse));
    ERROR_CHECK(ret,-1,"setsockopt");

    struct sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    ret = bind(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"bind");

    listen(sockfd,50);

    //处理链接
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd,-1,"epoll_create");
    //初始化监听集合sockfd,连接用
    struct epoll_event events;
    events.events = EPOLLIN;
    events.data.fd = sockfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&events);
    char buf[4096] = {0};
    int netfd = -1;
    while(1)
    {
        //初始化就序集合(监听用)
        struct epoll_event readySet[2];
        int readyNum = epoll_wait(epfd,readySet,sizeof(readySet),-1);
        ERROR_CHECK(readyNum,-1,"epoll_wait");
        //遍历就绪集合数组
        for(int i = 0;i < readyNum;++i)
        {
            if(netfd == -1 && readySet[i].data.fd == sockfd)
            {
                //新建连接
                struct sockaddr_in clientAddr;
                socklen_t socklen = sizeof(clientAddr); //必须初始化
                netfd = accept(sockfd,(struct sockaddr *)&clientAddr,&socklen);
                ERROR_CHECK(netfd,-1,"accept");
                printf("阿强已上线\n");
                //设置监听集合
                events.events = EPOLLIN;
                events.data.fd = netfd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,&events);
                events.events = EPOLLIN;
                events.data.fd = STDIN_FILENO;
                epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&events);
            }
            else if(netfd != -1 && readySet[i].data.fd == STDIN_FILENO)
            {
                bzero(buf,sizeof(buf));
                ssize_t sret = read(STDIN_FILENO,buf,sizeof(buf));
                //自己断开连接
                if(sret == 0)
                {
                    exit(0);
                }
                sret = send(netfd,buf,sret,0);
                ERROR_CHECK(sret,-1,"send");
            }
            else if(netfd != -1 && readySet[i].data.fd == netfd)
            {
                //阿强断开连接&接收消息
                bzero(buf,sizeof(buf));
                ssize_t sret = recv(netfd,buf,sizeof(buf),0);
                //断开连接
                if(sret == 0)
                {
                    printf("阿强断开连接！\n");
                    close(netfd);
                    //设置监听集合
                    events.events = EPOLLIN;
                    events.data.fd = netfd;
                    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd,NULL);
                    events.events = EPOLLIN;
                    events.data.fd = sockfd;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&events);
                    //重置netfd
                    netfd = -1;
                    break;
                }
                //接收消息
                printf("%s\n",buf);
            }
        }
    }
    return 0;
}


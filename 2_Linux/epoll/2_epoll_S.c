#include <54func.h>

typedef struct conn_s
{
    int isConnected;    //0:未连接，1：已连接
    time_t lastActive;
}Conn_t;

int main(int argc, char * argv[])
{
    // ./hw4_epoll_S 192.168.244.129 12345
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

    //创建epoll文件对象
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd,-1,"epoll_create");
    //设置监听集合sockfd
    struct epoll_event events;
    events.events = EPOLLIN;
    events.data.fd = sockfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&events);

    Conn_t netfd[1024] = {{0,0}};   //所有的netfd 下标代表fd
    int maxfd = sockfd;             //最大的fd
    char buf[4096] = {0};

    while(1)
    {
        //用户态就序集合数组
        struct epoll_event readySet[1024];
        int readyNum = epoll_wait(epfd,readySet,sizeof(readySet),1000);
        ERROR_CHECK(readyNum,-1,"epoll_wait");
        time_t now = time(NULL);
        //遍历就绪数组
        for(int i = 0;i < readyNum;++i)
        {
            if(readySet[i].data.fd == sockfd)
            {
                //新建连接
                struct sockaddr_in clientAddr;
                socklen_t socklen = sizeof(clientAddr); //必须初始化
                int newfd = accept(sockfd,(struct sockaddr *)&clientAddr,&socklen);
                ERROR_CHECK(newfd,-1,"accept");
                printf("%d 已上线\n",newfd);
                if(newfd > maxfd) {maxfd = newfd;}
                //通知其他人已上线
                bzero(buf,sizeof(buf));
                sprintf(buf,"%d 已上线\n",newfd);
                for(int j = 0; j <= maxfd; j++)
                {
                    if(netfd[j].isConnected == 0) { continue; }
                    ssize_t sret = send(j,buf,strlen(buf),0);
                    ERROR_CHECK(sret,-1,"send");
                }
                //更新监听集合
                events.events = EPOLLIN;
                events.data.fd = newfd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,newfd,&events);
                //更新已连接数组
                netfd[newfd].isConnected = 1;
                netfd[newfd].lastActive = time(NULL);
            }
            else
            {
                //其他情况
                bzero(buf,sizeof(buf));
                ssize_t sret = recv(readySet[i].data.fd,buf,sizeof(buf),0);
                //断开连接
                if(sret == 0)
                {
                    //更新监听集合
                    int delfd = readySet[i].data.fd;
                    epoll_ctl(epfd,EPOLL_CTL_DEL,delfd,NULL);
                    //更新已连接数组
                    netfd[delfd].isConnected = 0;
                    netfd[delfd].lastActive = 0;
                    close(delfd);
                    printf("%d 已下线\n",delfd);
                    if(delfd == maxfd) {--maxfd;}
                    //通知其他人
                    bzero(buf,sizeof(buf));
                    sprintf(buf,"%d 已下线\n",delfd);
                    for(int j = 0; j <= maxfd; j++)
                    {
                        if(netfd[j].isConnected == 0) { continue; }
                        ssize_t sret = send(j,buf,strlen(buf),0);
                        ERROR_CHECK(sret,-1,"send");
                    }
                    continue;
                }
                //发消息
                netfd[readySet[i].data.fd].lastActive = time(NULL);
                for(int j = 0; j <= maxfd; j++)
                {
                    if(netfd[j].isConnected == 0 || j == readySet[i].data.fd) { continue; }
                    sret = send(j,buf,sret,0);
                    ERROR_CHECK(sret,-1,"send");
                }
            }
        }
        //超时踢人
        for(int i = 0;i <= maxfd;++i)
        {
            if(netfd[i].isConnected == 1 && now - netfd[i].lastActive > 10)
            {
                close(i);
                //更新监听集合
                epoll_ctl(epfd,EPOLL_CTL_ADD,i,NULL);
                //更新已连接数组
                netfd[i].isConnected = 0;
                netfd[i].lastActive = 0;
                printf("%d 连接超时！\n",i);
            }
        }
    }
    return 0;
}


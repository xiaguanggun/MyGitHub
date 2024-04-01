#include <54func.h>

typedef struct conn_s
{
    int isConnected;    //0:未连接，1：已连接
    time_t lastActive;
}Conn_t;

int main(int argc, char * argv[])
{
    // ./hw6_server 192.168.244.129 54321
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
    fd_set monitorSet,readySet;     //监听集合，就序集合
    Conn_t netfd[1024] = {{0,0}};   //所有的netfd 下标代表fd
    int maxfd = sockfd;             //最大的fd
    char buf[4096] = {0};
    FD_ZERO(&monitorSet);
    FD_SET(sockfd,&monitorSet);
    
    //timeout超时
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    while(1)
    {
        //初始化就序集合(监听用)
        memcpy(&readySet,&monitorSet,sizeof(monitorSet));
        int ret = select(maxfd+1,&readySet,NULL,NULL,&timeout);
        ERROR_CHECK(ret,-1,"select");
        time_t now = time(NULL);
        //新建连接
        if(FD_ISSET(sockfd,&readySet))
        {
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
            FD_SET(newfd,&monitorSet);
            netfd[newfd].isConnected = 1;
            netfd[newfd].lastActive = time(NULL);
        }
        //其他情况
        for(int i = 0; i <= maxfd; i++)
        {
            if(netfd[i].isConnected == 0) { continue; }
            //需要监听的netfd
            if(FD_ISSET(i,&readySet))
            {
                bzero(buf,sizeof(buf));
                ssize_t sret = recv(i,buf,sizeof(buf),0);
                //断开连接
                if(sret == 0)
                {
                    FD_CLR(i,&monitorSet);
                    netfd[i].isConnected = 0;
                    netfd[i].lastActive = 0;
                    close(i);
                    printf("%d 已下线\n",i);
                    if(i == maxfd) {--maxfd;}
                    //通知其他人
                    bzero(buf,sizeof(buf));
                    sprintf(buf,"%d 已下线\n",i);
                    for(int j = 0; j <= maxfd; j++)
                    {
                        if(netfd[j].isConnected == 0) { continue; }
                        ssize_t sret = send(j,buf,strlen(buf),0);
                        ERROR_CHECK(sret,-1,"send");
                    }
                    continue;
                }
                //发消息
                netfd[i].lastActive = time(NULL);
                for(int j = 0; j <= maxfd; j++)
                {
                    if(netfd[j].isConnected == 0 || j == i) { continue; }
                    sret = send(j,buf,sret,0);
                    ERROR_CHECK(sret,-1,"send");
                }
            }
        }
        //超时踢人
        for(int i = 0;i <= maxfd;++i)
        {
            if(netfd[i].isConnected == 1 && now - netfd[i].lastActive > 5)
            {
                close(i);
                FD_CLR(i,&monitorSet);
                netfd[i].isConnected = 0;
                netfd[i].lastActive = 0;
                printf("%d 连接超时！\n",i);
            }
        }
    }
    return 0;
}


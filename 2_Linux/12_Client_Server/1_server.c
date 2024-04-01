#include <54func.h>

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
    fd_set monitorSet,readySet; //监听集合，就序集合
    int netfd[1024] = {0};      //所有的netfd 下标代表fd，0:未连接，1：已连接
    int maxfd = sockfd;              //最大的fd
    char buf[4096] = {0};
    FD_ZERO(&monitorSet);
    FD_SET(sockfd,&monitorSet);
    while(1)
    {
        //初始化就序集合(监听用)
        memcpy(&readySet,&monitorSet,sizeof(monitorSet));
        int ret = select(maxfd+1,&readySet,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
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
                if(netfd[j] == 0) { continue; }
                ssize_t sret = send(j,buf,strlen(buf),0);
                ERROR_CHECK(sret,-1,"send");
            }
            FD_SET(newfd,&monitorSet);
            netfd[newfd] = 1;
        }
        //其他情况
        for(int i = 0; i <= maxfd; i++)
        {
            if(netfd[i] == 0) { continue; }
            //需要监听的netfd
            if(FD_ISSET(i,&readySet))
            {
                bzero(buf,sizeof(buf));
                ssize_t sret = recv(i,buf,sizeof(buf),0);
                //断开连接
                if(sret == 0)
                {
                    FD_CLR(i,&monitorSet);
                    netfd[i] = 0;
                    close(i);
                    printf("%d 已下线\n",i);
                    //if(i == maxfd) {--maxfd;}
                    //通知其他人
                    bzero(buf,sizeof(buf));
                    sprintf(buf,"%d 已下线\n",i);
                    for(int j = 0; j <= maxfd; j++)
                    {
                        if(netfd[j] == 0) { continue; }
                        ssize_t sret = send(j,buf,strlen(buf),0);
                        ERROR_CHECK(sret,-1,"send");
                    }
                    continue;
                }
                //发消息
                for(int j = 0; j <= maxfd; j++)
                {
                    if(netfd[j] == 0 || j == i) { continue; }
                    sret = send(j,buf,sret,0);
                    ERROR_CHECK(sret,-1,"send");
                }
            }
        }
    }

    return 0;
}


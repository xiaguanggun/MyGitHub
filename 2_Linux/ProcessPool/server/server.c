#include "head.h"

int exitpipe[2];

void handler(int signum)
{
    printf("signum = %d\n",signum);
    write(exitpipe[1],"1",1);
}

int main(int argc, char * argv[])
{
    // ./server 192.168.244.129 12345 3
    ARGS_CHECK(argc,4);
    // makeWorker
    int workerNum = atoi(argv[3]);
    workerdata_t *workerArr = (workerdata_t *)calloc(workerNum,sizeof(workerdata_t));
    makeWorker(workerArr,workerNum);
    // 创建一条和自己通信的管道 注册10号信号
    pipe(exitpipe);
    signal(SIGUSR1,handler);
    // tcpInit
    int sockfd;
    tcpInit(argv[1],argv[2],&sockfd);
    // epoll
    // epoll_create
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd,-1,"epoll_create");
    // epoll_ctl
    for(int i = 0; i < workerNum; ++i)
    {
        epollAdd(epfd,workerArr[i].pipesockfd);
    }
    // 轮询
    int monitorSet[2] = {sockfd,exitpipe[0]};
    eventLoop(epfd,monitorSet,2,workerArr,workerNum);

    return 0;
}


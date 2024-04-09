#include <54func.h>
#include "epoll.h"
#include "threadPool.h"

// 父子进程通信管道
int exitPipe[2];

void handler(int signum)
{
    printf("signum = %d\n", signum);
    write(exitPipe[1],"1",1);
}

int tcpInit(const char *ip,const char *port, int *psockfd);

int main(int argc, char * argv[])
{
    // ./server ip port workernum
    ARGS_CHECK(argc,4);
    // 父子进程
    pipe(exitPipe);
    if(fork() != 0){
        // 父进程
        close(exitPipe[0]);
        signal(SIGUSR1,handler);
        wait(NULL);
        exit(0);
    }
    // 子进程
    close(exitPipe[1]);
    // threadPoolInit
    threadPool_t worker;
    threadPoolInit(&worker,atoi(argv[3]));
    // makeWorker
    makeWorker(&worker);
    // tcpInit
    int sockfd;
    tcpInit(argv[1],argv[2],&sockfd);
    // epollAdd
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd,-1,"epoll_create");
    epollAdd(epfd,sockfd);
    epollAdd(epfd,exitPipe[0]);
    // epoll_wait
    while(1){
        struct epoll_event readySet[1024];
        int readyNum = epoll_wait(epfd,readySet,sizeof(readySet),-1);
        for(int i = 0; i < readyNum; ++i){
            if(readySet[i].data.fd == exitPipe[0]){
                printf("线程池退出!\n");
                pthread_mutex_lock(&worker.mutex);
                worker.exitFlag = 1;
                pthread_cond_broadcast(&worker.cond);
                pthread_mutex_unlock(&worker.mutex);
                for(int i = 0;i < worker.workerArr.workerNum; ++i){
                    pthread_join(worker.workerArr.arr[i],NULL);
                }
                exit(0);
            }
            else if(readySet[i].data.fd == sockfd){
                int netfd = accept(sockfd,NULL,NULL);
                pthread_mutex_lock(&worker.mutex);
                enQueue(&worker.taskQueue,netfd);
                pthread_cond_signal(&worker.cond);
                pthread_mutex_unlock(&worker.mutex);
            }
        }
    }
    return 0;
}

int tcpInit(const char *ip,const char *port, int *psockfd)
{
    // socket
    *psockfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(*psockfd,-1,"socket");
    // setsockopt
    int reuse = 1;
    int ret = setsockopt(*psockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    // bind
    struct sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port));
    ret = bind(*psockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"bind");
    // listen
    listen(*psockfd,50);

    return 0;
}

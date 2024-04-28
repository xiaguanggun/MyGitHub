#include "threadPool.h"
#include "log.h"

int threadPoolInit(threadPool_t *pthreadPool,int workerNum)
{
    tidArrInit(&pthreadPool->tidArr,workerNum);
    taskQueueInit(&pthreadPool->taskQueue);
    pthread_mutex_init(&pthreadPool->mutex,NULL);
    pthread_cond_init(&pthreadPool->cond,NULL);
    pthreadPool->exitFlag = 0;
    return 0;
}

int tcpInit(const char *ip,const char *prot,int *psockfd)
{
    *psockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(prot));
    server.sin_addr.s_addr = inet_addr(ip);
    int reuse = 1;
    int ret = setsockopt(*psockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret,-1,"setsockopt");
    ret = bind(*psockfd,(struct sockaddr *)&server,sizeof(server));
    ERROR_CHECK(ret,-1,"bind");
    listen(*psockfd,50);
    return 0;
}


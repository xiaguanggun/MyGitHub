#include <54func.h>
#include "worker.h"
#include "threadPool.h"
#include "transFile.h"

//int makeWorker(workerdata_t *workerArr, int workerNum)
//{
//    for(int i = 0; i < workerNum; ++i)
//    {
//        //父子进程通信socketpair
//        int fds[2];
//        socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
//        pid_t pid = fork();
//        if(pid == 0)
//        {
//            //工人进程(子进程)
//            close(fds[1]);
//            //死循环
//            workLoop(fds[0]);
//        }
//        //主进程
//        close(fds[0]);
//        workerArr[i].pid = pid;
//        workerArr[i].stat = FREE;
//        workerArr[i].pipesockfd = fds[1];
//        printf("idx = %d,pid = %d,stat = %d, pipesockfd = %d\n",
//               i,workerArr[i].pid,workerArr[i].stat,workerArr[i].pipesockfd);
//    }
//    return 0;
//}
//
//int workLoop(int sockfd)
//{
//    while(1)
//    {
//        int idx;
//        int netfd;                  //与客户端通信的sockfd
//        int exitFlag;
//        recvfd(sockfd,&netfd,&idx,&exitFlag); //接受任务
//        if(exitFlag == 1)
//        {
//            printf("worker eixt!\n");
//            exit(0);
//        }
//        printf("work begin!\n");
//        //发送文件
//        transFile(netfd);
//        close(netfd);
//        printf("work end!\n");
//        //任务完成后给主进程发消息
//        send(sockfd,&idx,sizeof(idx),0);
//    }
//    return 0;
//}

int workerArrInit(workerArr_t *pworkerArr, int workerArrNum)
{
    pworkerArr->arr = (pthread_t *)calloc(workerArrNum,sizeof(pthread_t));
    pworkerArr->workerNum = workerArrNum;
    return 0;
}

void *threadFunc(void *arg)
{
    threadPool_t * pthreadPool = (threadPool_t *)arg; 
    // 业务
    while(1){
        pthread_mutex_lock(&pthreadPool->mutex);
        while(pthreadPool->taskQueue.taskQueueSize <= 0 && 
              pthreadPool->exitFlag == 0){
            pthread_cond_wait(&pthreadPool->cond,&pthreadPool->mutex);
        }
        if(pthreadPool->exitFlag == 1){
            pthread_mutex_unlock(&pthreadPool->mutex);
            pthread_exit(NULL);
        }
        int netfd = pthreadPool->taskQueue.pFront->netfd;
        deQueue(&pthreadPool->taskQueue);
        pthread_mutex_unlock(&pthreadPool->mutex);
        transFile(netfd);
    }
}


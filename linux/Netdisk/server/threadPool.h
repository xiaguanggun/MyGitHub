#ifndef __THREADPOOL__
#define __THREADPOOL__
#include "54func.h"
#include "worker.h"
#include "taskQueue.h"

typedef struct threadPool_s{
    tidArr_t tidArr;//存储子线程信息
    taskQueue_t taskQueue;//任务队列
    pthread_mutex_t mutex;//锁
    pthread_cond_t cond;//条件变量
    int exitFlag;//退出标志位，0表示未退出状态，1表示退出状态
}threadPool_t;

int threadPoolInit(threadPool_t *pthreadPool,int workerNum);
int tcpInit(const char *ip,const char *prot,int *psockfd);
int makeWorker(threadPool_t *pthreadpool);
#endif

#ifndef __THREADPOOL__
#define __THREADPOOL__

#include "worker.h"
#include "taskQueue.h"

typedef struct threadPool_s
{
    workerArr_t workerArr;
    taskQueue_t taskQueue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int exitFlag;
} threadPool_t;

int threadPoolInit(threadPool_t *pthreadPool, int workerNum);

int makeWorker(threadPool_t *pthreadPool);

#endif


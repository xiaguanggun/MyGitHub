#include <54func.h>
#include "threadPool.h"
#include "worker.h"

int threadPoolInit(threadPool_t *pthreadPool, int workerNum)
{
    workerArrInit(&pthreadPool->workerArr,workerNum);
    taskQueueInit(&pthreadPool->taskQueue);
    pthread_mutex_init(&pthreadPool->mutex,NULL);
    pthread_cond_init(&pthreadPool->cond,NULL);
    pthreadPool->exitFlag = 0;
    return 0;
}

int makeWorker(threadPool_t *pthreadPool)
{
    printf("makeWorker!\n");
    for(int i = 0; i < pthreadPool->workerArr.workerNum; ++i){
        pthread_create(&pthreadPool->workerArr.arr[i],NULL,threadFunc,pthreadPool);
    }
    return 0;
}

#include "threadPool.h"

int taskQueueInit(taskQueue_t *pQueue)
{
    memset(pQueue,0,sizeof(taskQueue_t));
    return 0;
}
int enQueue(taskQueue_t *pQueue,int netfd)
{
    node_t *pNew = (node_t *)calloc(1,sizeof(node_t));
    pNew->netfd = netfd;
    if(pQueue->queueSize == 0){
        pQueue->pFront = pNew;
        pQueue->pRear = pNew;
    }
    else{
        pQueue->pRear->pNext = pNew;
        pQueue->pRear = pNew;
    }
    pQueue->queueSize++;
    return 0;
}
int deQueue(taskQueue_t *pQueue)
{
    pQueue->pFront = pQueue->pFront->pNext;
    if(pQueue->queueSize == 1){
        pQueue->pRear = NULL;
    }
    pQueue->queueSize--;
    return 0;
}

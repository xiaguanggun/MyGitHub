#include <54func.h>
#include "taskQueue.h"

int taskQueueInit(taskQueue_t *ptaskQueue)
{
    bzero(ptaskQueue,sizeof(taskQueue_t));
    return 0;
}

int enQueue(taskQueue_t *ptaskQueue, int netfd)
{
    node_t *pNew = (node_t *)calloc(1,sizeof(node_t));
    pNew->netfd = netfd;
    if(ptaskQueue->taskQueueSize == 0)
    {
        ptaskQueue->pRear = pNew;
        ptaskQueue->pFront = pNew;
    }
    else
    {
        ptaskQueue->pRear->pNext = pNew;
    }
    ptaskQueue->pRear = pNew;
    ++ptaskQueue->taskQueueSize;
    return 0;
}

int deQueue(taskQueue_t *ptaskQueue)
{
    // 队列非空才能调用deQueue
    node_t *pCur = ptaskQueue->pFront;
    ptaskQueue->pFront = pCur->pNext;
    if(ptaskQueue->taskQueueSize == 1)
    {
        ptaskQueue->pRear = NULL;
    }
    free(pCur);
    --ptaskQueue->taskQueueSize;
    return 0;
}


#ifndef __TASKQUEUE__
#define __TASKQUEUE__

typedef struct node_s
{
    int netfd;
    struct node_s *pNext;
} node_t;

typedef struct taskQueue_s
{
    node_t *pFront;
    node_t *pRear;
    int taskQueueSize;
} taskQueue_t;

int taskQueueInit(taskQueue_t *ptaskQueue);
int enQueue(taskQueue_t *ptaskQueue, int netfd);
int deQueue(taskQueue_t *ptaskQueue);

#endif


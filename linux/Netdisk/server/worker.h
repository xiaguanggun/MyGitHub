#ifndef __WORKER__
#define __WORKER__
#include "54func.h"

typedef struct tidArr_s{
    pthread_t *arr;
    int workerNum;
}tidArr_t;

int tidArrInit(tidArr_t *ptidArr,int workerNum);
void *threadFunc(void *arg);

#endif

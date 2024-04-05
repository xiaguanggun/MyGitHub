#ifndef __WORKER__
#define __WORKER__

#include <54func.h>

typedef struct workerArr_s{
    pthread_t *arr;
    int workerNum;
} workerArr_t;

int workerArrInit(workerArr_t *pworkerArr, int workerArrNum);

void *threadFunc(void *arg);

#endif


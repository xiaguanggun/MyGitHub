#include <54func.h>

void *threadFunc(void *arg){
int *pVal = (int *)arg;
    printf("child, *pVal = %d\n", *pVal);
    ++*pVal;
    return NULL;
}

int main()
{
    int *pVal = (int *)malloc(sizeof(int));
    ERROR_CHECK(pVal,NULL,"malloc");
    *pVal = 1000;
    printf("*pVal = %d\n",*pVal);

    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc, pVal);
    THREAD_ERROR_CHECK(ret,"pthread_create");

    free(pVal);
    pVal = NULL;

    sleep(2);

    return 0;
}

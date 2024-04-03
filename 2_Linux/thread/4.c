#include <54func.h>
#define NUM 10000000

void *threadFunc(void *arg)
{
    int *pi = (int *)arg;
    for(int i = 0; i < NUM; i++)
    {
        ++*pi;
    }
    return NULL;
}

int num = 0;

int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,&num);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    for(int i = 0; i < NUM; i++)
    {
        ++num;
    }
    sleep(2);
    printf("num = %d\n",num);
    return 0;
}


#include <54func.h>
#define NUM 20000000

typedef struct shareRes_s
{
    int i;
    pthread_mutex_t mutex;
}shareRes_t;

void *threadFunc(void *arg)
{
    shareRes_t *shareRes_p  = (shareRes_t *)arg;
    for(int i = 0;i < NUM;i++)
    {
        pthread_mutex_lock(&shareRes_p->mutex);
        ++shareRes_p->i;
        pthread_mutex_unlock(&shareRes_p->mutex);
    }
    pthread_exit(NULL);
}

int main()
{
    shareRes_t arg;
    arg.i = 0;
    pthread_mutex_init(&arg.mutex,NULL);
    
    pthread_t tid;
    struct timeval beginTime,endTime;
    gettimeofday(&beginTime,NULL);
    int ret = pthread_create(&tid,NULL,threadFunc,&arg);
    THREAD_ERROR_CHECK(ret,"pthread_create");

    for(int i = 0;i < NUM;i++)
    {
        pthread_mutex_lock(&arg.mutex);
        ++arg.i;
        pthread_mutex_unlock(&arg.mutex);
    }

    pthread_join(tid,NULL);
    gettimeofday(&endTime,NULL);

    printf("total time = %ld us\n",(endTime.tv_sec-beginTime.tv_sec)*1000*1000 + endTime.tv_usec - beginTime.tv_usec);

    return 0;
}


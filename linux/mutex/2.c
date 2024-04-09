#include <54func.h>

typedef struct shareRes_s
{
    int flag;
    pthread_mutex_t mutex;
}shareRes_t;

void *threadFunc(void *arg)
{
    shareRes_t *shareRes_p = (shareRes_t *)arg;

    while(1)
    {
        pthread_mutex_lock(&shareRes_p->mutex);
        if(shareRes_p->flag) {break;}
        pthread_mutex_unlock(&shareRes_p->mutex);
    }

    printf("Before B!\n");
    sleep(3);
    printf("After B\n");
    pthread_exit(NULL);
}

int main()
{
    shareRes_t shareRes;
    shareRes.flag = 0;
    pthread_mutex_init(&shareRes.mutex,NULL);

    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,&shareRes);
    THREAD_ERROR_CHECK(ret,"pthread_create");

    printf("Before A!\n");
    sleep(3);
    printf("After A\n");
    pthread_mutex_lock(&shareRes.mutex);
    shareRes.flag = 1;
    pthread_mutex_unlock(&shareRes.mutex);

    pthread_join(tid,NULL);

    return 0;
}


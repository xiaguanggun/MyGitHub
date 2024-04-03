#include <54func.h>

typedef struct shareRes_s
{
    int flag;   // 0 都没完成 1 A已完成 2 B已完成
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}shareRes_t;

void *threadFunc1(void *arg)
{
    shareRes_t *pshareRes = (shareRes_t *)arg;
    //直接执行A,之后修改flag
    printf("A\n");
    pthread_mutex_lock(&pshareRes->mutex);
    pshareRes->flag = 1;
    pthread_cond_signal(&pshareRes->cond);
    pthread_mutex_unlock(&pshareRes->mutex);
    //flag != 2 wait
    pthread_mutex_lock(&pshareRes->mutex);
    while(pshareRes->flag != 2)
    {
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
    }
    pthread_mutex_unlock(&pshareRes->mutex);
    printf("C\n");
    pthread_exit(NULL);
}
void *threadFunc2(void *arg)
{
    shareRes_t *pshareRes = (shareRes_t *)arg;
    //flag != 1 wait
    pthread_mutex_lock(&pshareRes->mutex);
    while(pshareRes->flag != 1)
    {
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
    }
    pthread_mutex_unlock(&pshareRes->mutex);
    pthread_mutex_lock(&pshareRes->mutex);
    printf("B\n");
    pshareRes->flag = 2;
    pthread_cond_signal(&pshareRes->cond);
    pthread_mutex_unlock(&pshareRes->mutex);
    pthread_exit(NULL);
}

int main()
{
    shareRes_t shareRes;
    shareRes.flag = 0;
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_cond_init(&shareRes.cond,NULL);

    pthread_t tid1,tid2;
    int ret = pthread_create(&tid1,NULL,threadFunc1,&shareRes);
    THREAD_ERROR_CHECK(ret,"pthread_create1");
    ret = pthread_create(&tid2,NULL,threadFunc2,&shareRes);
    THREAD_ERROR_CHECK(ret,"pthread_create2");

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    return 0;
}


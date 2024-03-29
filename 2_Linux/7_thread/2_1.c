#include <54func.h>

void *threadFunc(void *arg){
int *pVal = (int *)arg;
    printf("child, *pVal = %d\n", *pVal);
    ++*pVal;
    return NULL;
}

void func(){
    int val = 1000;
    pthread_t tid;
    pthread_create(&tid,NULL,threadFunc, &val);
}

int main()
{
    func();
    sleep(1);

    return 0;
}

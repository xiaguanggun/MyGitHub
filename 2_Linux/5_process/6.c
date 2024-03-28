#include <54func.h>

#define NUM 10000000

int main()
{
    //向OS申请
    int shmid = shmget(0x1234,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    //申请虚拟内存
    int *p = (int *)shmat(shmid,NULL,0);
    ERROR_CHECK(p,NULL,"shmat");
    *p = 0;
    if(fork())
    {
        //父进程
        for(int i = 0; i < NUM; i++)
        {
            ++*p;
        }
        wait(NULL);
        printf("*p = %d\n", *p);
    }
    else
    {
        //子进程
        for(int i = 0; i < NUM; i++)
        {
            ++*p;
        }
    }
    shmdt(p);
    return 0;
}


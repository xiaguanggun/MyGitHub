#include <54func.h>

int main()
{
    //向OS申请
    int shmid = shmget(0x1234,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    //申请虚拟内存
    char *p = (char *)shmat(shmid,NULL,0);
    ERROR_CHECK(p,NULL,"shmat");
    sprintf(p,"How are you");
    shmdt(p);
    return 0;
}


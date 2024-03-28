#include <54func.h>

int main()
{
    //打开半双工管道
    int pipefd[2];
    int ret = pipe(pipefd);
    ERROR_CHECK(ret,-1,"pipe");
    if(fork())
    {
        //父进程
        //调整为单工管道
        close(pipefd[0]);
        //W端
        int count = 1;
        while(1)
        {
            char buf[4096] = {0};
            write(pipefd[1],buf,sizeof(buf));
            printf("write × %d\n",count++);
            sleep(1);
        }
    }
    else
    {
        //子进程
        //调整为单工管道
        close(pipefd[1]);
        //R端
        int count = 1;
        while(1)
        {
            char buf[1024];
            read(pipefd[0],buf,sizeof(buf));
            printf("read × %d\n",count++);
            sleep(1);
        }
    }
    return 0;
}


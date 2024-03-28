#include <54func.h>

void Daemon()
{
    //新建会话
    if(fork())
    {
        exit(0);
    }
    setsid();
    //关闭之前的设备
    for(int i = 0; i < 1024; i++)
    {
        close(i);
    }
    //修改环境属性
    umask(0);
    chdir("/");
}

int main()
{
    //创建守护进程
    Daemon();
    //子进程(守护进程)
    for(int i = 0; i < 5; i++)
    {
       syslog(LOG_INFO,"I am Daemon,i = %d\n",i);
       sleep(1);
    }
    return 0;
}


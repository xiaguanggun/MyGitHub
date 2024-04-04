#include "head.h"

int epollAdd(int epfd,int fd)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    return 0;
}

int epollDel(int epfd,int fd)
{
    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
    return 0;
}

int eventLoop(int epfd, int monitorSet[], int monitorNum, workerdata_t *workerArr, int workerNum)
{
    for(int i = 0; i < monitorNum; ++i)
    {
        epollAdd(epfd, monitorSet[i]);
    }
    while(1)
    {
        struct epoll_event readySet[1024];
        int readyNum = epoll_wait(epfd,readySet,sizeof(readySet),-1);
        for(int i = 0; i < readyNum; ++i)
        {
            if(readySet[i].data.fd == monitorSet[0])
            {
                // 新任务到来
                int netfd = accept(monitorSet[0],NULL,NULL);
                ERROR_CHECK(netfd,-1,"accept in epoll.c");
                printf("New client connected!\n");
                //查找空闲子进程
                for(int i = 0; i < workerNum; ++i)
                {
                    if(workerArr[i].stat == FREE)
                    {
                        int exitFlag = NORMAL;
                        sendfd(workerArr[i].pipesockfd,netfd,i,exitFlag);
                        workerArr[i].stat = BUSY;
                        break;
                    }
                }
                close(netfd);
            }
            else if(readySet[i].data.fd == monitorSet[1])
            {
                // 退出进程池
                for(int i = 0; i < workerNum; ++i)
                {
                    sendfd(workerArr[i].pipesockfd,0,i,KILL);
                    printf("kill 1 worker!\n");
                }
                for(int j = 0; j < workerNum; ++j)
                {
                    // 回收子进程资源
                    wait(NULL);
                }
                printf("All workers exited!\n");
                // 退出父进程
                exit(0);
            }
            else
            {
                // 某个工人子进程完成任务了
                int idx;
                ssize_t sret = recv(readySet[i].data.fd,&idx,sizeof(idx),0);
                if(sret == 0)
                {
                    printf("idx = %d,pid = %d,exit!\n",idx,workerArr[idx].pid);
                    exit(-1);
                }
                printf("idx = %d,pid = %d,task finished!\n",idx,workerArr[idx].pid);
                workerArr[idx].stat = FREE;
            }
        }
    }
    return 0;
}

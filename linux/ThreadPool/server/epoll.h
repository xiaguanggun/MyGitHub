#ifndef __EPOLL__
#define __EPOLL__

#include "worker.h"

int epollAdd(int epfd,int fd);
int epollDel(int epfd,int fd);

#endif


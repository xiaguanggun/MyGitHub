#ifndef __TRANSFILE__
#define __TRANSFILE__

// 小火车
typedef struct train_s
{
    int length;
    char data[1000];
} train_t;

int recvn(int netfd, void *buf, int length);

int transFile(int netfd);

#endif


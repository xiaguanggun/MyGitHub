#include <54func.h>

enum
{
    FREE,
    BUSY
};

enum
{
    NORMAL,
    KILL
};

// 小火车
typedef struct train_s
{
    int length;
    char data[1000];
} train_t;

// 工人进程信息
typedef struct workerdata_s
{
    pid_t pid;
    int stat;       // 状态：FREE BUSY
    int pipesockfd; // 主进程与工人进程通信的sockfd
} workerdata_t;

// 函数接口
int makeWorker(workerdata_t *workerArr, int workerNum);
int tcpInit(const char *ip,const char *port, int *psockfd);
int epollAdd(int epfd, int fd);
int epollDel(int epfd, int fd);
int eventLoop(int epfd, int monitorSet[],int monitorNum, workerdata_t *workerArr, int workerNum);
int sendfd(int sockfd, int fdtosend, int idx, int exitFlag);
int recvfd(int sockfd, int *pfdtorecv, int *pidx, int *pexitFlag);
int workLoop(int sockfd);
int recvn(int netfd, void *buf, int length);
int transFile(int netfd);

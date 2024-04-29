#include "serverFunc.h"
#include "assistFuncServer.h"
#include "threadPool.h"
#include "worker.h"

// 数据库用户名密码
extern char *sqlName;
extern char *sqlPwd;

int tidArrInit(tidArr_t *ptidArr,int workerNum)//初始化子线程信息结构体
{
    ptidArr->arr = (pthread_t *)calloc(workerNum,sizeof(pthread_t));
    ptidArr->workerNum = workerNum;
    return 0;
}

int makeWorker(threadPool_t *pthreadPool)//创建工人进程
{
    for(int i = 0;i < pthreadPool->tidArr.workerNum;i++){
        pthread_create(&pthreadPool->tidArr.arr[i],NULL,threadFunc,pthreadPool);
    }
    return 0;
}

//子线程函数
void *threadFunc(void *arg)
{
    // 获取参数
    threadPool_t * pthreadPool = (threadPool_t *)arg;
    // 初始化数据库连接
    pthread_mutex_lock(&pthreadPool->mutex);
    MYSQL* conn= mysql_init(NULL);
    MYSQL* cret = mysql_real_connect(conn,"localhost", sqlName, sqlPwd,"netdisk",0,NULL,0);
    if(NULL == cret){
        printf("子线程数据库连接失败!\n");
        printf("%s\n",mysql_error(conn));
        exit(0);
    }
    pthread_mutex_unlock(&pthreadPool->mutex);
    // 业务
    while(1){
        int netfd;
        cmd_t cmd;
        // 获取任务
        pthread_mutex_lock(&pthreadPool->mutex);
        while(pthreadPool->exitFlag == 0 && 
              pthreadPool->taskQueue.queueSize <= 0){
            pthread_cond_wait(&pthreadPool->cond,&pthreadPool->mutex);
        }
        if(pthreadPool->exitFlag == 1){
            pthread_mutex_unlock(&pthreadPool->mutex);
            pthread_exit(NULL);
        }
        netfd = pthreadPool->taskQueue.pFront->netfd;
        deQueue(&pthreadPool->taskQueue);
        pthread_mutex_unlock(&pthreadPool->mutex);
        // 执行任务
        // 1 收cmd_t
        recvn(netfd,&cmd,sizeof(cmd_t));
        // 2 执行指令
        if(strcmp(cmd.cmdToken,"puts") == 0) {
            serverGets(conn,netfd,&cmd);
        }
        else if(strcmp(cmd.cmdToken,"gets") == 0){
            serverPuts(conn,netfd,&cmd);
        }
    }
    // 关闭数据库连接
    mysql_close(conn);
}

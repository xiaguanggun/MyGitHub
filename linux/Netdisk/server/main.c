#include "54func.h"
#include "serverFunc.h"
#include "threadPool.h"
#include "epoll.h"
#include "worker.h"
#include "assistFuncServer.h"
#include "log.h"
#include "wheel.h"

// 全局变量
// 网盘的根目录
char rootDir[128] = {0};
// 日志系统目录
char log_addr[128] = {0};
// mysql账户密码
char *sqlName;
char *sqlPwd;

// 父子进程通信管道
int exitPipe[2];
// 信号注册函数
void handler(int signum)
{
    printf("signum = %d\n",signum);
    write(exitPipe[1],"1",1);
}

// 处理超时的环形队列的相关变量及函数定义
// uid与其所在环形队列中slot的映射
int uidMap[MAX_UID_SIZE] = {-1};
// 用于记录环形队列中每一个slot中，某一连接是否存在
int wheel[WHEEL_SIZE][MAX_UID_SIZE] = {0};
// 当前循环所在的slot
int currIdx = 0;
// 用于记录当前最大的文件描述符
int maxFd = 0;
// 更新当前最大的文件描述符
static int updateMaxFd(int fd) {
    if(fd > maxFd) {
        maxFd = fd;
    }
    return 0;
}

// ./server ../Conf/server.conf
int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2);
    //启动服务端时创建网盘根目录和日志系统文件
    //拼接家目录
    char tempDir[256] = {0};
    char homeDir[64] = {0};
    char initDir1[128] = {0};
    char initDir2[128] = {0};
    getcwd(tempDir,sizeof(tempDir));
    int homeDirLength = 0;
    int ret = 0;
    for(size_t i = 0; i < strlen(tempDir); ++i){
        if(tempDir[i] == '/') { ++ret; }
        ++homeDirLength;
        if(ret == 3) { break;  }
    }
    memcpy(homeDir,tempDir,homeDirLength);
    printf("homeDir = %s\n",homeDir);
    sprintf(initDir1,"%snetdisk/",homeDir);
    sprintf(rootDir,"%snetdisk/three/",homeDir);
    sprintf(initDir2,"%snetdisk/three/log/",homeDir);
    sprintf(log_addr,"%snetdisk/three/log/my_log",homeDir);
    // 创建目录
    ret = mkdir(initDir1,0777);
    ret = mkdir(rootDir,0777);
    ret = mkdir(initDir2,0777);
    ret = open(log_addr,O_RDWR|O_CREAT,0777);
    close(ret);
    if (ret != -1) {
        printf("网盘初始化成功!");
        fflush(stdout);
    } else {
        perror("网盘初始化失败!\n");
        exit(0);
    }

    //获取配置文件中的ip，port，workerNum,sqlName,sqlPwd
    int fd = open(argv[1],O_RDONLY);
    if(fd == -1) { printf("配置文件不存在!\n"); }
    char buf[1024] = {0};
    ret = read(fd,buf,sizeof(buf));
    if(ret == -1) { printf("读取配置文件失败!\n"); }
    char *ip = strtok(buf," \n");
    char *port = strtok(NULL," \n");
    char *workerNum = strtok(NULL," \n");
    sqlName = strtok(NULL," \n");
    sqlPwd = strtok(NULL," \n");
    if(sqlPwd == NULL) { printf("配置文件内容出错!\n"); }

    //检查数据库中是否有netdisk(database)和user filesystem这两个表
    MYSQL* conn = mysql_init(NULL);
    MYSQL* cret =  mysql_real_connect(conn,"localhost",sqlName,sqlPwd,"information_schema",0,NULL,0);
    const char *err = mysql_error(conn);
    printf("%s\n",err);
    if(cret == NULL ){
        printf("主线程数据库连接失败!\n");
        return -1;
    }
    // 查询netdisk
    mysql_query(conn,"select SCHEMA_NAME from information_schema.SCHEMATA where SCHEMA_NAME = 'netdisk';");
    SQL_LOG(conn);
    MYSQL_RES *res = mysql_store_result(conn);
    SQL_LOG(conn);
    MYSQL_ROW row;
    if((row = mysql_fetch_row(res)) == NULL){
        //不存在则创建
        mysql_query(conn,"create database netdisk;");
        SQL_LOG(conn);
        printf("Database:netdisk创建成功!\n");
    }
    // use netdisk
    mysql_query(conn,"use netdisk;");
    SQL_LOG(conn);
    // 创建表
    mysql_query(conn,"create table if not exists user("
                "id int auto_increment primary key,"
                "name varchar(20) unique not NULL,"
                "salt char(20) not NULL,"
                "encrypted_password varchar(255) not NULL,"
                "tomb int default 0 not NULL,"
                "column1 int,column2 int,column3 int);");
    SQL_LOG(conn);
    printf("user表初始化成功!\n");
    mysql_query(conn,"create table if not exists filesystem("
                "id int auto_increment primary key,"
                "filename varchar(255) not NULL,"
                "user varchar(20) not NULL,"
                "preid int not NULL,"
                "path varchar(4096) not NULL,"
                "type char(1) not NULL,"
                "sha1 char(40),"
                "tomb int default 0 not NULL,"
                "column1 int,column2 int,column3 int);");
    SQL_LOG(conn);
    printf("filesystem表初始化成功!\n");

    // 父子进程
    pipe(exitPipe);
    if(fork() != 0){
        close(exitPipe[0]);
        signal(SIGUSR1,handler);
        wait(NULL);
        printf("父进程退出!\n");
        exit(0);
    }
    close(exitPipe[1]);
    threadPool_t threadPool;
    threadPoolInit(&threadPool,atoi(workerNum));
    makeWorker(&threadPool);
    pthread_mutex_lock(&threadPool.mutex);
    pthread_mutex_unlock(&threadPool.mutex);

    int sockfd;
    tcpInit(ip,port,&sockfd);
    int epfd = epoll_create(1);
    epollAdd(epfd,sockfd);
    epollAdd(epfd,exitPipe[0]);
    while(1){
        struct epoll_event readySet[1024];
        int readyNum = epoll_wait(epfd,readySet,1024, 1000);
        timeTick(epfd);
        cmd_t cmd;
        for(int i = 0;i < readyNum;i++){
            if(readySet[i].data.fd == exitPipe[0]){
                printf("线程池退出!\n");
                pthread_mutex_lock(&threadPool.mutex);
                threadPool.exitFlag = 1;
                pthread_cond_broadcast(&threadPool.cond);
                pthread_mutex_unlock(&threadPool.mutex);
                for(int i = 0;i < threadPool.tidArr.workerNum;i++){
                    pthread_join(threadPool.tidArr.arr[i],NULL);
                }
                printf("子进程主线程退出!\n");
                exit(0);
            }
            else if(readySet[i].data.fd == sockfd) {
                // 需要判断是新客户端还是收发文件请求
                // 1 建立连接
                int netfd = accept(sockfd,NULL,NULL);
                // 2 收"e"或"o"
                train_t train;
                if(recvMsg(netfd,&train) == 1){
                    // 2.1 "o",新客户端,加入监听
                    epollAdd(epfd,netfd);
                    receiveUidRequest(netfd);                    
                    updateMaxFd(netfd);
                }
                else {
                    // 2.2 "e"+msg,收发文件
                    pthread_mutex_lock(&threadPool.mutex);
                    enQueue(&threadPool.taskQueue,netfd);
                    printf("我是主线程,我发送了一个任务,netfd = %d\n",netfd);
                    pthread_cond_signal(&threadPool.cond);
                    pthread_mutex_unlock(&threadPool.mutex);
                }
            }
            else {
                // 其他情况:已连接客户端主线程发送指令
                // 1 收cmd_t
                ret = recvn(readySet[i].data.fd,&cmd,sizeof(cmd));
                if(ret == -1){
                    //客户端关闭
                    epollDel(epfd,readySet[i].data.fd);
                    close(readySet[i].data.fd);
                }
                // 2 调用handler,处理收发文件之外的指令
                cmdServerHandler(conn,readySet[i].data.fd,&cmd);
            }
        }
    }
    mysql_close(conn);
    SQL_LOG(conn);
    return 0;
}

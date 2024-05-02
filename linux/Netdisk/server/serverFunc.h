#ifndef __SERVERFUNC__
#define __SERVERFUNC__

#include "54func.h"
#define TRAINLEN 1000
#define TOKENLEN 512
#define ARGVLEN 256

// server的实际根目录
extern char rootDir[128];
extern char log_addr[128];

// 结构体定义
// 用于接收C的字符串cmd
typedef struct {
    char cmdToken[8];
    char user[20];
    char token[TOKENLEN];
    int argc;
    char argv1[ARGVLEN];
    char argv2[ARGVLEN];
} cmd_t;

// 小火车
typedef struct train_s
{
    int length;
    char data[TRAINLEN];
} train_t;


// token认证函数
// token加密函数
int generateJwt(cmd_t* pcmd);
// token解码函数
int verifyJwt(cmd_t* pcmd);

// 指令函数
// 传入cmd_t，解析并执行相应的指令函数,传入cwd_t为指令函数传参
int cmdServerHandler(MYSQL* conn, int netfd, cmd_t *pcmd);
// cd指令,修改cwd栈,cmd_t获取指令及参数
int cdCmd(MYSQL* conn, int netfd, const cmd_t *pcmd);
// ls指令,cwd_t获取cwd,cmd_t获取指令及参数
int lsCmd(MYSQL* conn, int netfd, const cmd_t *pcmd);
// rm指令,cwd_t获取cwd,cmd_t获取指令及参数
int rmCmd(MYSQL *conn, int netfd, const cmd_t *pcmd);
// mkdir指令,cwd_t获取cwd,cmd_t获取指令及参数
int mkdirCmd(MYSQL *conn, int netfd, cmd_t *pcmd);

// 发送文件,cwd_t获取cwd,cmd_t获取指令及参数
int serverPuts(MYSQL *conn, int netfd, const cmd_t *pcmd);
// 接收文件,cwd_t获取cwd,cmd_t获取指令及参数
int serverGets(MYSQL *conn, int netfd, cmd_t *pcmd);

// 登录用户,cmd_t获取指令及参数
int userLogin(MYSQL *conn, int netfd, cmd_t *pcmd);
// 创建用户,cmd_t获取指令及参数
int userAdd(MYSQL *conn, int netfd, cmd_t *pcmd);
// 删除用户,cmd_t获取指令及参数
int userDel(MYSQL *conn, int netfd, const cmd_t *pcmd);

#endif

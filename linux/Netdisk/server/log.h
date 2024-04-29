#ifndef __LOG__
#define __LOG__
#include "54func.h"
// 全局变量,日志文件地址
extern char log_addr[128];


// 用宏函数调用mylog函数
// 记录mysql的错误信息
#define SQL_LOG(conn) {char msg[4096];{mylog_mysql(conn,__FILE__,__FUNCTION__,__LINE__,msg);if (msg != NULL && msg[0] != '\0'){return -1;}}}
int mylog_mysql(MYSQL* conn, const char *filePath, const char *func, int line,char *msg);

//记录客户端操作信息
#define CLIENT_LOG(msg) {mylog(msg);}
int mylog(const char *msg);


//记录perror的错误信息

#define ERROR_CHECK(ret,num,msg) {if(ret == num){error_check(msg);return -1;}}
int error_check(const char *msg);


#endif

#ifndef __ASSISTFUNCSERVER__
#define __ASSISTFUNCSERVER__

#include "54func.h"
#include "serverFunc.h"


// 辅助函数
// recvn                                                                                                                                   
int recvn(int netfd, void *buf, int length);
// 判断路径是否是传入用户所拥有的合法路径
// 返回值: 1(路径正确), 0(路径不正确) 
int pathJudge(MYSQL* conn, const char* realPath, const char* user, const char *type);
// 接收确认信息'e'或'o', 信息为'o'时返回1, 为'e'时读取报错信息, 返回0
int recvMsg(int sockfd, train_t* ptrain);
// 用小火车发送字符串
int strSend(int netfd, train_t* ptrain, const char* str);
// 发送成功信息'o'
int successSend(int netfd);
// 发送错误信息, 如果填入NULL, 发送一个'e'报错信号
// 如果填入字符串, 将其作为报错信息发送给客户端
int errorSend(int netfd, const char* errorMsg);
// 获取待新增的文件或目录的preid, 存储在*ppreid中
int getPreid(MYSQL* conn, const char* user, const char* filePath, int* ppreid);
// 在目录路径后添加文件名,filePath传入目录路径,argv为文件路径
//                       ,filePath传入空字符串时只获取文件名
int concatFileName(const char *argv,char *filePath);
// filesystem插表操作
int insertFileTable(MYSQL *conn,cmd_t *pcmd,char *sha1Num);

#endif

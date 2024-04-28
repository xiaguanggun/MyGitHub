#include "assistFuncServer.h"
#include "log.h"

// 辅助函数
// recvn                                                                                                                                   
int recvn(int netfd, void *buf, int length)
{
    char *p = (char *)buf;
    while(length > 0){
        ssize_t sret = recv(netfd,p,length,0);
        ERROR_CHECK(sret,-1,"recv in recvn");
        if(sret == 0){
            return -1;
        }
        p += sret;
        length -= sret;
    }
    return 0;
}
// 判断路径是否是传入用户所拥有的合法路径
// 返回值: 1(路径正确), 0(路径不正确) 
int pathJudge(MYSQL* conn, const char* realPath, const char* user, const char* type) {
    char sql[4096] = {0};
    bool isValid = false;
    sprintf(sql, "select * from filesystem where "
            "path = '%s' and user = '%s' and type = '%s' and tomb = 0;",
            realPath, user, type);
    mysql_query(conn, sql);
    SQL_LOG(conn);
    MYSQL_RES* res;
    res = mysql_store_result(conn);
    int ret = 0;
    if((ret = mysql_num_rows(res)) != 0) {
        isValid = true;
    }
    mysql_free_result(res);
    if(isValid) {
        return 1;
    }
    return 0;
}
// 接收确认信息'e'或'o', 信息为'o'时返回1, 为'e'时读取报错信息, 返回0
int recvMsg(int sockfd, train_t* ptrain) {
    char msgFlag[1] = {0};
    recvn(sockfd, msgFlag, sizeof(msgFlag));
    if(msgFlag[0] == 'e') {
        memset(ptrain, 0, sizeof(train_t));
        recvn(sockfd, &ptrain->length, sizeof(int));
        recvn(sockfd, ptrain->data, ptrain->length);
        printf("%s\n", ptrain->data);
        return 0;
    }
    return 1;
}
// 用小火车发送字符串
int strSend(int netfd, train_t* ptrain, const char* str) {
    memset(ptrain, 0, sizeof(train_t));
    ptrain->length = strlen(str) + 1;
    memcpy(ptrain->data, str, ptrain->length);
    send(netfd, ptrain, ptrain->length + sizeof(int), MSG_NOSIGNAL);
    return 0;
}

// 发送成功信息'o'
int successSend(int netfd) {
    send(netfd, "o", 1, MSG_NOSIGNAL);
    return 0;
}
// 发送错误信息, 如果填入NULL, 发送一个'e'报错信号
// 如果填入字符串, 将其作为报错信息发送给客户端
int errorSend(int netfd, const char* errorMsg) {
    if(errorMsg == NULL) {
        send(netfd, "e", 1, MSG_NOSIGNAL);
    }
    else {
        train_t train;
        strSend(netfd, &train, errorMsg);
    }
    return 0;
}
// 获取待新增的文件或目录的preid, 存储在*ppreid中
int getPreid(MYSQL* conn, const char* user, const char* filePath, int* ppreid)
{
    char sql[4096];
    // 去掉末尾的待新增的文件或目录名
    char path[2048] = {0};
    int filePathSize = strlen(filePath);
    while(filePathSize > 0){
        if(filePath[filePathSize-1] != '/') { --filePathSize; }
        else { break; }
    }
    if(filePathSize>1 && filePath[filePathSize-1] == '/'){
        --filePathSize;
    }
    memcpy(path,filePath,filePathSize);
    // 查表
    memset(sql,0,sizeof(sql));
    sprintf(sql,"select id from filesystem where "
            "user = '%s' and path = '%s' and tomb = 0;",user,path);
    //printf("sql = %s\n",sql);
    mysql_query(conn,sql);
    SQL_LOG(conn);
    MYSQL_RES *res;
    res = mysql_store_result(conn);
    if(mysql_num_rows(res) <= 0) {
        // 未找到
        printf("未查询到preid!\n");
        return -1;
    }    
    // 找到并修改*ppreid
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    *ppreid = atoi(row[0]);
    mysql_free_result(res);
    return 0;
}
// 在目录路径后添加文件名,filePath传入目录路径,argv为文件路径
//                       ,filePath传入空字符串时只获取文件名
int concatFileName(const char *argv,char *filePath)
{
    int filePathSize = strlen(filePath);
    int argvSize = strlen(argv);
    while(argvSize > 0){
        if(argv[argvSize-1] != '/') { --argvSize; }
        else { break; }
    }
    if(filePathSize >0 && filePath[filePathSize-1] != '/'){
        sprintf(filePath+filePathSize,"%s","/");
        ++filePathSize;
    }
    sprintf(filePath+filePathSize,"%s",argv+argvSize);
    return 0;
}
// filesystem插表操作
int insertFileTable(MYSQL *conn,cmd_t *pcmd,char *sha1Num)
{
    char sql[4096];
    // 获取文件名
    char fileName[256] = {0};
    concatFileName(pcmd->argv1,fileName);
    // 获取preid
    int preid;
    if(strcmp(pcmd->cmdToken,"useradd") != 0) {
        if(strlen(pcmd->argv2) == 1){
            concatFileName(pcmd->argv1,pcmd->argv2);
        }
        getPreid(conn,pcmd->user,pcmd->argv2,&preid);
    }
    // 组合指令sql
    memset(sql,0,sizeof(sql));
    if(strcmp(pcmd->cmdToken,"useradd") == 0) {
        sprintf(sql,"insert into filesystem "
                "(filename,user,preid,path,type) "
                "values ('/','%s',-1,'/','d');",pcmd->argv1);
    }
    else{
        sprintf(sql,"insert into filesystem "
                "(filename,user,preid,path,type,sha1) "
                "values ('%s','%s',%d,'%s','f','%s');",
                fileName,pcmd->user,preid,pcmd->argv2,sha1Num);
    }
    // 插入表
    mysql_query(conn,sql);
    SQL_LOG(conn);
    return 0;
}


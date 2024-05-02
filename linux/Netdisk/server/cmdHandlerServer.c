#include "54func.h"
#include "serverFunc.h"
#include "assistFuncServer.h"
#include "log.h"

// 传入cmd_t，解析并执行相应的指令函数,传入cwd_t为指令函数传参
int cmdServerHandler(MYSQL* conn, int netfd, cmd_t *pcmd) {
    if((pcmd->user[0] != '\0') && verifyJwt(pcmd) == 0) {
        printf("token 解码失败，无法操作!\n");
    }    
    char msg[4096] = {0};
    const char *userName;
    if(strlen(pcmd->user) == 0){
        userName = "未登录";
    }else {
        userName = pcmd->user;
    }
    if(pcmd->argc == 1){
        sprintf(msg,"%s:%s %s",userName,pcmd->cmdToken,pcmd->argv1);
    }
    else if(pcmd->argc == 2) {
        sprintf(msg,"%s:%s %s %s",userName,pcmd->cmdToken,pcmd->argv1,pcmd->argv2);
    }
    else {
        sprintf(msg,"%s:%s",userName,pcmd->cmdToken);
    }
    CLIENT_LOG(msg);

    switch(pcmd->cmdToken[0]) {
    case'c': cdCmd(conn, netfd, pcmd); break; 
    case'l':
             switch(pcmd->cmdToken[1]) {
             case's':lsCmd(conn, netfd, pcmd); break;
             case'o':userLogin(conn,netfd,pcmd);break;
             }
             break;
    case'u':
             switch(pcmd->cmdToken[4]){
             case'a':userAdd(conn,netfd, pcmd);break;
             case'd':userDel(conn,netfd,pcmd);break;
             }
             break;
    case'm': mkdirCmd(conn, netfd, pcmd); break;
    case'r': rmCmd(conn, netfd, pcmd); break;
    }
    return 0;
}


// cd指令,修改cwd栈,cmd_t获取指令及参数
int cdCmd(MYSQL* conn, int netfd, const cmd_t *pcmd) {
    if(pathJudge(conn, pcmd->argv1, pcmd->user, "d")) {
        // cd 路径存在
        successSend(netfd);
    }
    else {
        // 错误
        errorSend(netfd, NULL);
        errorSend(netfd, "cd执行失败,路径无效!");
    }
    return 0;
}

// 将整个目录流中的内容发送给客户端
static int sendDir(MYSQL* conn, int netfd, const char* path, const char* user) {
    char sql[1024] = {0};
    char id[10] = {0};
    MYSQL_ROW row;
    train_t train;
    sprintf(sql, "select id from filesystem where "
            "path = '%s' and user = '%s' and tomb = 0;", path, user);

    mysql_query(conn, sql);

    MYSQL_RES* res = mysql_store_result(conn);
    if(mysql_num_rows(res) == 0);
    else {
        row = mysql_fetch_row(res);
        memcpy(id, row[0], sizeof(id));
        mysql_free_result(res);
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select filename from filesystem where "
                "preid = %s and tomb = 0;", id);
        mysql_query(conn, sql);
        res = mysql_store_result(conn);
        while((row = mysql_fetch_row(res)) != NULL) {
            strSend(netfd, &train, row[0]);
        }
    }
    memset(&train, 0, sizeof(train_t));
    send(netfd, &train, sizeof(int) + train.length, MSG_NOSIGNAL);
    return 0;
}

// ls指令,cwd_t获取cwd,cmd_t获取指令及参数
int lsCmd(MYSQL* conn, int netfd, const cmd_t *pcmd) {
    if(pathJudge(conn, pcmd->argv1, pcmd->user, "d")) {
        successSend(netfd);
        sendDir(conn, netfd, pcmd->argv1, pcmd->user);
    }
    else {
        errorSend(netfd,NULL);
        errorSend(netfd, "路径无效!");
    }
    return 0;
}


#include "serverFunc.h"
#include "assistFuncServer.h"
#include "log.h"
#define SQLLEN 4096

// 递归删除目录及里面的所有文件,rm -r
// 辅助递归函数
static int recursiveRm(MYSQL *conn, char *sql, int preid)
{
    //static int num = 0;
    //++num;
    //printf("num = %d\n",num);
    // 查找目标id和type
    memset(sql,0,SQLLEN);
    sprintf(sql,"select id,type from filesystem where "
            "preid = %d and tomb = 0;",preid);
    mysql_query(conn,sql);
    SQL_LOG(conn);
    MYSQL_RES *res = mysql_store_result(conn);
    if(mysql_num_rows(res) <= 0){
        // 递归出口
        return 0;
    }
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        int id = atoi(row[0]);
        char type = row[1][0];
        // 直接删除自身
        memset(sql,0,SQLLEN);
        sprintf(sql,"update filesystem set tomb = 1 where id = %d;",id);
        mysql_query(conn,sql);
        SQL_LOG(conn);
        // 是文件就直接返回
        if(type == 'f'){
            return 0;
        }
        // 目录还需要调用递归函数
        if(recursiveRm(conn, sql, id) != 0){
            return -1;
        }
    }
    return 0;
}
// 删除指令
int rmCmd(MYSQL *conn, int netfd, const cmd_t *pcmd)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char sql[SQLLEN];
    // 查找目标id和type
    memset(sql,0,SQLLEN);
    sprintf(sql,"select id,type from filesystem where "
            "path = '%s' and user = '%s' and tomb = 0;"
            ,pcmd->argv1,pcmd->user);
    mysql_query(conn,sql);
    SQL_LOG(conn);
    res = mysql_store_result(conn);
    if(mysql_num_rows(res) <= 0){
        // 文件或目录不存在
        errorSend(netfd,NULL);
        errorSend(netfd,"文件或目录不存在!");
        return -1;
    }
    // 文件存在
    row = mysql_fetch_row(res);
    int id = atoi(row[0]);
    char type = row[1][0];
    // 直接删除自身
    memset(sql,0,SQLLEN);
    sprintf(sql,"update filesystem set tomb = 1 where id = %d;",id);
    mysql_query(conn,sql);
    SQL_LOG(conn);
    // 是文件就直接返回
    if(type == 'f'){
        successSend(netfd);
        return 0;
    }
    // 目录还需要调用递归函数
    if(recursiveRm(conn, sql, id) == 0){
        successSend(netfd);
        return 0;
    }
    else{
        errorSend(netfd,NULL);
        errorSend(netfd,"删除失败!");
        return -1;
    }
}



// 从文件路径分离出文件名
int cutStr(char* str1, char* str2, char div) {
    int len = strlen(str1);
    if(len >= 1 && str1[len - 1] == div) {
        str1[len - 1] = '\0';
    }
    for(int i = len - 1; i >= 0; --i) {
        if(str1[i] == div || i ==  0) {
            strncpy(str2, str1 + i + 1, len - i);
            memset(str1 + i, 0, len - i);
            break;
        }
    }
    str1[0] = '/';
    return 0;
}

int mkdirCmd(MYSQL *conn, int netfd, cmd_t *pcmd){
    char newdir[255]={0};
    cutStr(pcmd->argv1, newdir, '/');

    int predir_id;
    getPreid(conn, pcmd->user, pcmd->argv1, &predir_id);


    char sql[2048] = {0};
    sprintf(sql, "select id from filesystem where "
            "path = '%s' and user = '%s' and "
            "tomb = 0 and type = 'd';", pcmd->argv1, pcmd->user);
    mysql_query(conn, sql);
    SQL_LOG(conn);

    MYSQL_RES* res;
    MYSQL_ROW row;
    int fatherId = 0;
    res = mysql_store_result(conn);
    SQL_LOG(conn); 
    if(mysql_num_rows(res) != 0) {
        row = mysql_fetch_row(res);
        fatherId = atoi(row[0]);
        //printf("fatherId = %d\n", fatherId);
    }
    else {
        errorSend(netfd, NULL);
        errorSend(netfd, "无法在不存在的地址创建文件夹");
        return -1;
    }
    mysql_free_result(res);
    memset(sql, 0, sizeof(sql));
    if(strcmp(pcmd->argv1, "/") == 0) {
        sprintf(sql, "insert into filesystem (filename, user, preid, path, type) "
                "values('%s', '%s', %d, '%s%s', 'd');"
                , newdir, pcmd->user, fatherId, pcmd->argv1, newdir);
    }
    else {
        sprintf(sql, "insert into filesystem (filename, user, preid, path, type) "
                "values('%s', '%s', %d, '%s/%s', 'd');"
                , newdir, pcmd->user, fatherId, pcmd->argv1, newdir);
    }
    //printf("%s\n", sql);
    SQL_LOG(conn);
    if(mysql_query(conn, sql) == 0) {
        successSend(netfd);
    }
    else {
        errorSend(netfd, NULL);
        errorSend(netfd, "创建文件夹失败\n");
    }
    return 0;
}


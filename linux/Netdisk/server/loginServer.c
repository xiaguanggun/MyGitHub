#include "54func.h"
#include "assistFuncServer.h"
#include "log.h"

int userLogin(MYSQL* conn, int netfd, cmd_t *pcmd){
    const char *username = pcmd->argv1;
    const char *password = pcmd->argv2;   

    char sql[1024];
    sprintf(sql, "SELECT salt,encrypted_password,tomb "
            "FROM user WHERE name = '%s';", username);

    // 执行查询
    mysql_query(conn,sql);
    SQL_LOG(conn);

    MYSQL_RES *res;
    res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        errorSend(netfd, NULL);
        errorSend(netfd, "用户不存在!");
        mysql_free_result(res);
        return -1 ;
    }

    // 获取行数据
    MYSQL_ROW row = mysql_fetch_row(res);
    char* salt = row[0];
    char* encryptedPassword = row[1];
    char tomb = row[2][0];
    if(tomb == '1'){
        // 已注销
        errorSend(netfd,NULL);
        errorSend(netfd,"用户已注销!");
        return -1;
    }

    // 使用收到的明文密码和盐值生成密文
    char* generatedPassword = crypt(password, salt);

    // 将生成的密文与数据库中的密文进行比较
    if (strncmp(generatedPassword, encryptedPassword, strlen(generatedPassword)) == 0) {        
        train_t train;
        strcpy(pcmd->user, pcmd->argv1);
        generateJwt(pcmd);
        successSend(netfd);
        strSend(netfd, &train, pcmd->token);

        mysql_free_result(res);
    }
    else {
        errorSend(netfd, NULL);
        errorSend(netfd, "密码错误!");
        mysql_free_result(res);
        return -1;
    }

    return 0;
}

#include "log.h"


//服务端的日志系统
//记录客户端的连接时间、操作记录、操作时间、其余报错信息
//保存到日志文件
//需要的参数：
//调用日志函数的时候需要显示调用位置的行数等信息所以要把__LINE__（当前行）、__FUNCTION__（当前所在函数）、__FILE__（当前源文件的文件名（包括路径））  
//这三个宏的返回值当成参数传入日志函数
//所以参数如下：char *opt,const char*file,const char *func,int line



//记录mysql报错信息
int mylog_mysql(MYSQL* conn, const char *filePath, const char *func, int line,char *msg){
    sprintf(msg,"%s", mysql_error(conn));
    if (msg != NULL && msg[0] != '\0'){
        //说明发生了错误
        FILE* file= fopen(log_addr,"ab+");
        if(file==NULL){
            perror("fopen");
            return -1;
        }
        time_t cur;
        time(&cur);//获取当前时间戳到cur当中
        struct tm *info;//存本地时间
        info=localtime(&cur);
        fprintf(file,"当前时间:%d-%02d-%02d %02d:%02d:%02d  ",
                info->tm_year+1900,info->tm_mon+1,info->tm_mday,info->tm_hour,info->tm_min,info->tm_sec);
        fprintf(file,"mysql信息:%s  位置信息:%s,%s,%d\n",msg,filePath,func,line - 1);
        fclose(file);
        return 0;
    }
    return 0;
}



//记录客户端操作信息
int mylog(const char *msg)
{
    FILE* file= fopen(log_addr,"ab+");
    if(file==NULL){
        perror("fopen");
        return -1;
    }
    time_t cur;
    time(&cur);//获取当前时间戳到cur当中
    struct tm *info;//存本地时间
    info=localtime(&cur);
    fprintf(file,"当前时间:%d-%02d-%02d %02d:%02d:%02d  ",
            info->tm_year+1900,info->tm_mon+1,info->tm_mday,info->tm_hour,info->tm_min,info->tm_sec); 
    fprintf(file,"客户端操作:%s\n",msg);
    fclose(file);
    return 0;
}


//还需要把perror打印的错误信息写入日志
//每次调用perror后调用perror_to_log把错误信息也写入日志当中
//需要参数：perror调用时传的参数
//先把标准错误输出重定向到日志文件，在重定向回去

//记录perror错误信息
int error_check(const char *msg){
    int log_fd= open(log_addr,O_RDWR|O_CREAT|O_APPEND,0755);
    if(log_fd==-1){
        printf("打开日志文件失败!\n");
        return 0;
    }
    int tmp=dup(STDERR_FILENO);//备份标准错误输出文件描述符
    if(tmp==-1){
        perror("dup");
    }
    int ret= dup2(log_fd,STDERR_FILENO);
    if(ret==-1){
        perror("dup2");
    }
    perror(msg);
    int ret1= dup2(tmp,STDERR_FILENO);
    if(ret1==-1){
        perror("dup2");
    }
    close(log_fd);
    return 0;
}


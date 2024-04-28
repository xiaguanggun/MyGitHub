#include "54func.h"
#include "clientFunc.h"

// 全局变量结构体定义,标识目前是否有用户登录以及用户名
//typedef struct loginInfo_s
//{
//    int loginFlag;      // 0:无人登录 1:有人登录
//    char userName[20];  // 无人登录时全为0,有人登录时保存对应用户名
//} loginInfo_t;
loginInfo_t loginInfo;
// cwd栈
cwd_t cwd;
// 存储服务器ip+port
addrInfo_t addrInfo;

int main(int argc, char *argv[])
{
    // ./client ip port
    // TCP通信
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    strcpy(addrInfo.ip, argv[1]);
    strcpy(addrInfo.port, argv[2]);
    int ret = connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"connect");

    // "o"新建客户端发送一个字符
    successSend(sockfd);

    int epfd = epoll_create(1);
    struct epoll_event events;

    events.events = EPOLLIN;
    events.data.fd = STDIN_FILENO;
    epoll_ctl(epfd,EPOLL_CTL_ADD, STDIN_FILENO, &events);

    //初始化需要的参数
    cmd_t cmd;
    bzero(&loginInfo,sizeof(loginInfo));
    char buf[4096] = {0};
    memset(&cwd, 0, sizeof(cwd_t));
    memset(&cmd, 0, sizeof(cmd_t));

    //开始业务
    while(1){
        // 如果处于已登录状态,就打印前缀:用户名->
        if(loginInfo.loginFlag == 1) {
            printf("%s->请输入指令:",loginInfo.userName);
            fflush(stdout);
        }
        else {
            printf("->请输入指令:");
            fflush(stdout);
        }
        // 等待用户输入
        struct epoll_event readySet[1];
        epoll_wait(epfd,readySet,sizeof(readySet),-1);
        if(readySet[0].data.fd == STDIN_FILENO){
            memset(buf,0,sizeof(buf));
            fgets(buf,sizeof(buf),stdin);
            // 处理stdin输入的最后的\n,替换成空格
            int length = strlen(buf);
            if(length <= 1) {   //只有\n
                continue;
            }
            else if(length < 3) {
                printf("指令错误请重新输入!\n");
                continue;
            }
            if(buf[length-1] == '\n') {
                buf[length-1] = 0;
            }
            // 解析指令
            memset(&cmd,0,sizeof(cmd_t));
            if(parseToken(buf,&cmd) == 0) {
                // 指令解析正确,发送cmd到Handler函数
                memcpy(cmd.user, loginInfo.userName, sizeof(cmd.user));
                memcpy(cmd.token, loginInfo.token, sizeof(cmd.token));
                cmdClientHandler(sockfd, &cmd);
            }
        }
    }
    return 0;
}

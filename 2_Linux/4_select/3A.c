#include <54func.h>

int main(int argc, char * argv[])
{
    // ./LD08_3_A 1.pipe 2.pipe
    ARGS_CHECK(argc,3);
    int pfdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(pfdw,-1,"open_pipe1_write");
    int pfdr = open(argv[2], O_RDONLY);
    ERROR_CHECK(pfdr,-1,"open_pipe2_read");
    printf("A is connecting.\n");
    fd_set readfds;
    time_t lasttime = time(NULL);
    char buf[1024] = {0};

    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(pfdr, &readfds);
        FD_SET(STDIN_FILENO,&readfds);
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        select(pfdr+1,&readfds,NULL,NULL,&timeout);
        if(time(NULL)-lasttime >= 10)
        {
            write(pfdw,"You Lost!\n",10);
            break;
        }
        if(FD_ISSET(pfdr,&readfds))
        {
            memset(buf,0,sizeof(buf));
            size_t rret = read(pfdr,buf,sizeof(buf)-1);
            //对端断开连接
            if(rret == 0)
            {
                printf("B is disconnected.\n");
                break;
            }
            time_t time_num = time(NULL);
            //更新对端的最后活跃时间
            lasttime = time_num;
            printf("时间：%s\nB:%s\n", ctime(&time_num),buf);
        }
        if(FD_ISSET(STDIN_FILENO,&readfds))
        {
            memset(buf,0,sizeof(buf));
            ssize_t ret = read(STDIN_FILENO,buf,sizeof(buf));
            //ctrl+d 主动退出
            if(ret == 0)
            {
                write(pfdw,"I dont want talk with you.\n",28);
                break;
            }
            write(pfdw,buf,ret);
        }
    }

    return 0;
}


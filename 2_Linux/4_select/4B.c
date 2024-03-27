#include <54func.h>

int main(int argc, char * argv[])
{
    // ./LD08_4_B 1.pipe
    ARGS_CHECK(argc,2);
    int pfdr = open(argv[1],O_RDONLY);
    ERROR_CHECK(pfdr,-1,"open_pipe_read");
    printf("pipe is opened.\n");

    char buf[4096];
    //协议
    //文件名文件大小文件内容全都分开传送
    //文件名用1024字符数组传输，大小用off_t
    bool file_name = true, file_size = true;
    fd_set readfds;
    char name[1024] = {0};
    off_t size = 0;
    int fd = -1;
    sleep(5);
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(pfdr,&readfds);
        select(pfdr+1,&readfds,NULL,NULL,NULL);
        if(FD_ISSET(pfdr,&readfds))
        {
            if(file_name)
            {
                read(pfdr,name,sizeof(name));
                file_name = false;
                continue;
            }
            if(file_size)
            {
                read(pfdr,&size,sizeof(off_t));
                //创建目录及文件
                mkdir("newdir",0777);
                chdir("./newdir/");
                fd = open(name,O_WRONLY|O_CREAT|O_EXCL,0777);
                ERROR_CHECK(fd,-1,"open new file");

                file_size = false;
                continue;
            }
            memset(buf,0,sizeof(buf));
            ssize_t ret = read(pfdr,buf,sizeof(buf));
            if(ret == 0)
            {
                close(fd);
                printf("文件传输完毕!\n");
                break;
            }
            write(fd,buf,ret);
        }
    }
    close(pfdr);
    return 0;
}


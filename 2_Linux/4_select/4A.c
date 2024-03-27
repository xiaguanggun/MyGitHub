#include <54func.h>

int main(int argc, char * argv[])
{
    // ./LD08_4_A 1.pipe file
    ARGS_CHECK(argc,3);
    int pfdw = open(argv[1],O_WRONLY);
    ERROR_CHECK(pfdw,-1,"open_pipe_write");
    printf("pipe is opened.\n");
    int fd = open(argv[2],O_RDONLY);
    ERROR_CHECK(fd,-1,"open_file");
    printf("file is opened.\n");
    //文件大小statbuf.st_size
    struct stat statbuf;
    int sret = stat(argv[2], &statbuf);
    ERROR_CHECK(sret,-1,"stat");

    char buf[4096];
    //协议
    //文件名文件大小文件内容全都分开传送
    //文件名用1024字符数组传输，大小用off_t
    bool file_name = true, file_size = true;
    fd_set writefds;
    while(1)
    {
        FD_ZERO(&writefds);
        FD_SET(pfdw,&writefds);
        select(pfdw+1,NULL,&writefds,NULL,NULL);
        if(FD_ISSET(pfdw,&writefds))
        {
            if(file_name)
            {
                write(pfdw,argv[2],1024);
                file_name = false;
                continue;
            }
            if(file_size)
            {
                write(pfdw,&statbuf.st_size,sizeof(off_t));
                file_size = false;
                continue;
            }
            memset(buf,0,sizeof(buf));
            ssize_t ret = read(fd,buf,sizeof(buf));
            if(ret == 0)
            {
                printf("文件传输完毕!\n");
                break;
            }
            write(pfdw,buf,ret);
        }
    }
    close(fd);
    close(pfdw);
    return 0;
}


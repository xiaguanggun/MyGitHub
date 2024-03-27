#include <54func.h>

int main(int argc, char * argv[])
{
    // ./LD08_2 1.pipe
    ARGS_CHECK(argc,2);
    int fdr = open(argv[1],O_RDWR);
    ERROR_CHECK(fdr, -1, "open_1");
    int fdw = open(argv[1],O_RDWR);
    ERROR_CHECK(fdw, -1, "open_1");

    fd_set readfds;
    fd_set writefds;

    char buf[4096]  = {0};
    int cnt = 0;

    while(1)
    {
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(fdr, &readfds);
        FD_SET(fdw, &writefds);
        memset(buf,0,sizeof(buf));
        int ret = select(fdw+1,&readfds,&writefds,NULL,NULL);
        if(FD_ISSET(fdr,&readfds))
        {
            read(fdr,buf,1024);
            printf("read success! ret = %d, cnt = %d\n", ret, cnt++);
        }
        if(FD_ISSET(fdw,&writefds))
        {
            write(fdw,buf,4096);
            printf("write success! ret = %d, cnt = %d\n", ret, cnt++);
        }
        sleep(1);
    }

    return 0;
}


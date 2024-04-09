#include <54func.h>

#define BUFF_LEN 1000000 
int main(int argc, char * argv[])
{
    // ./LD07_3 file
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0666);
    ERROR_CHECK(fd, -1, "open");
    char buf[BUFF_LEN];
    memset(buf,'1',sizeof(buf));
    write(fd,buf,sizeof(buf));
    close(fd);
    return 0;
}


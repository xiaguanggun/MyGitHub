#include <54func.h>

int main(int argc, char * argv[])
{
    // ./LD07_2 file1 file2
    ARGS_CHECK(argc,3);
    int fd1 = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd1, -1, "open_file1");

    int fd2 = open(argv[2], O_RDONLY);
    ERROR_CHECK(fd2, -1, "open_file2");

    char buf1[4096];
    char buf2[4096];
    
    while(1)
    {
        ssize_t sret1 = read(fd1,buf1,sizeof(buf1));
        ssize_t sret2 = read(fd2,buf2,sizeof(buf2));
        if(sret1 != sret2)
        {
            printf("两个文件内容不一致！\n");
            break;
        }
        if(memcmp(buf1, buf2, sret1) != 0)
        {
            printf("两个文件内容不一致！\n");
            break;
        }
        if(sret1 == 0)
        {
            printf("两个文件内容一致！\n");
            break;
        }
    }
    close(fd1);
    close(fd2);
    return 0;
}


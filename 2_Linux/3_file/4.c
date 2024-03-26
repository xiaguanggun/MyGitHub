#include <54func.h>

int main(int argc, char * argv[])
{
    // ./LD07_4 file
    ARGS_CHECK(argc,2);
    int fd = open(argv[1], O_RDWR|O_CREAT,0666);
    ERROR_CHECK(fd, -1, "open");
    off_t length = lseek(fd, 0, SEEK_END);
    ERROR_CHECK(length, -1, "lseek");
    lseek(fd, 0, SEEK_SET);
    ftruncate(fd,length);
    char *mret = (char *)mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(mret,MAP_FAILED,"mmap");

    for(int i = 0; i < length; i++)
    {
        if(mret[i] >= 'a' && mret[i] <= 'z')
        {
            mret[i] -= 32;
        }
        else if(mret[i] == '\"' || mret[i] == '\'' ||
                mret[i] == ','  || mret[i] == '.'  ||
                mret[i] == ';'  || mret[i] == '?')
        {
            mret[i] = ' ';
        }
    }
    munmap(mret, length);
    close(fd);

    return 0;
}


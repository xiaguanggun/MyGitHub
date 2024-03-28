#include <54func.h>

int main(int argc, char * argv[])
{
    // ./hw4_1 exefile
    ARGS_CHECK(argc,2);
    if(fork())
    {
        wait(NULL);
    }
    else
    {
        char buf[1024] = {0};
        sprintf(buf,"./%s",argv[1]);
        execl(argv[1],buf,NULL);
    }
    return 0;
}


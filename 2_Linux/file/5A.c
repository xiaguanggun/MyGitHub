#include <54func.h>

int main(int argc, char * argv[])
{
    // ./LD07_5A 5.pipe
    ARGS_CHECK(argc,2);
    int fdp = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdp, -1, "open_pipe_write");

    printf("Helloworld\n");

    sleep(1);
    int newfd = dup(STDOUT_FILENO);
    dup2(fdp,STDOUT_FILENO);
    printf("Helloworld\n");
    
    sleep(1);
    dup2(newfd,STDOUT_FILENO);
    printf("Helloworld\n");

    close(newfd);
    close(fdp);

    return 0;
}


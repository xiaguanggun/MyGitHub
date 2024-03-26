#include <54func.h>

int main(int argc, char * argv[])
{
    // ./LD07_5B 5.pipe
    ARGS_CHECK(argc,2);
    int fdp = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdp, -1, "open_pipe_read");
    
    char buf[1024] = {0};
    read(fdp, buf, sizeof(buf));
    printf("%s", buf);

    sleep(2);
    close(fdp);

    return 0;
}


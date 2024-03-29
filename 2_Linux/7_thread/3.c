#include <54func.h>

void * threadFunc(void *arg)
{
    sleep(1);
    int fd = *((int *)arg);
    char buf[1024] = {0};
    ssize_t ret = read(fd,buf,sizeof(buf));
    ERROR_CHECK(ret,-1,"read");
    printf("ret = %ld, buf = %s\n",ret,buf);
    return NULL;
}

int main(int argc, char * argv[])
{
    // ./hw4 file
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    write(fd,"hello world",11);
    lseek(fd,0,SEEK_SET);

    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,&fd);
    THREAD_ERROR_CHECK(ret,"pthread_create");

    //sleep(2);

    close(fd);
    sleep(5);
    return 0;
}


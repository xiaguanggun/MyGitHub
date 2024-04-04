#include <54func.h>

// 小火车
typedef struct train_s
{
    int length;
    char data[1000];
} train_t;
// recvn
int recvn(int netfd, void *buf, int length)
{
    char *p = (char *)buf;
    while(length > 0)
    {
        ssize_t sret = recv(netfd,p,length,0);
        ERROR_CHECK(sret,-1,"recv in recvn");
        if(sret == 0)
        {
            return -1;
        }
        p += sret;
        length -= sret;
    }
    return 0;
}

// 1.0 非mmap版本
int recvFile(int sockfd)
{
    train_t train;
    // fileName
    char fileName[4096] = {0};
    recvn(sockfd,&train.length,sizeof(train.length));
    recvn(sockfd,train.data,train.length);
    memcpy(fileName,train.data,train.length);
    printf("filename is %s\n",fileName);
    // fileSize
    off_t fileSize;
    recvn(sockfd,&train.length,sizeof(train.length));
    recvn(sockfd,train.data,train.length);
    memcpy(&fileSize,train.data,train.length);
    printf("filesize  = %ld\n",fileSize);
    // file 内容
    int filefd = open(fileName,O_RDWR|O_CREAT|O_TRUNC,0666);
    ERROR_CHECK(filefd,-1,"open");
    printf("filefd = %d\n",filefd);
    // 进度条数据
    int slice = fileSize / 1000;
    int curSize = 0;
    int totalSize = 0;
    while(1)
    {
        // recv
        recvn(sockfd,&train.length,sizeof(train.length));
        if(train.length == 0)   // 读完就退出循环
        {
            break;
        }
        recvn(sockfd,train.data,train.length);
        // write
        ssize_t sret = write(filefd,train.data,train.length);
        ERROR_CHECK(sret,-1,"write");
        // 进度条
        curSize += train.length;
        totalSize += train.length;
        if(curSize > slice)
        {
            float finished = totalSize * 100.0f / fileSize;
            int count = finished / 5;
            printf("已下载：%3.1f%%\t|", finished);
            for(int i = 0; i < count; ++i)
            {
                printf("#");
            }
            for(int j = 0; j < 20-count; ++j)
            {
                printf("=");
            }
            printf("|");
            if(finished == 100.0f)
            {
                printf("\n");
            }
            else
            {
                printf("\r");
            }
            fflush(stdout);
        }
    }
    close(filefd);
    return 0;
}
// 2.0 mmap版本
//int recvFile(int sockfd)
//{
//    train_t train;
//    // fileName
//    char fileName[4096] = {0};
//    recvn(sockfd,&train.length,sizeof(train.length));
//    recvn(sockfd,train.data,train.length);
//    memcpy(fileName,train.data,train.length);
//    // fileSize
//    off_t fileSize;
//    recvn(sockfd,&train.length,sizeof(train.length));
//    recvn(sockfd,train.data,train.length);
//    memcpy(&fileSize,train.data,train.length);
//    // file 内容
//    int filefd = open(fileName,O_RDWR|O_CREAT|O_TRUNC,0666);
//    ERROR_CHECK(filefd,-1,"open");
//    // 先修改文件大小
//    ftruncate(filefd,fileSize);
//    // mmap
//    char *p = (char *)mmap(NULL,fileSize,PROT_READ|PROT_WRITE,MAP_SHARED,filefd,0);
//    ERROR_CHECK(p,MAP_FAILED,"mmap");
//    
//    recvn(sockfd,p,fileSize);
//
//    munmap(p,fileSize);
//    close(filefd);
//    return 0;
//}

int main(int argc, char * argv[])
{
    // ./client 192.168.244.129 12345
    // 192.168.244.129 12345 是S的ip&port
    ARGS_CHECK(argc,3);
    //socket
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket");
    //connect
    struct sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    int ret = connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"connect");
    //recvFile
    recvFile(sockfd);

    close(sockfd);
    return 0;
}


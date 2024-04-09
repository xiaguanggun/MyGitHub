#include <54func.h>
#include "transFile.h"

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

// 1.0 非mmap版
int transFile(int netfd)
{
    char filename[] = "file1";
    train_t train;
    // 发送文件名
    train.length = strlen(filename);
    memcpy(train.data,filename,train.length);
    send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    // 打开文件并发送
    int filefd = open(filename,O_RDWR);
    ERROR_CHECK(filefd,-1,"open in transFile");
    // 发送文件长度
    struct stat statbuf;
    fstat(filefd,&statbuf);
    train.length = sizeof(statbuf.st_size);
    memcpy(train.data,&statbuf.st_size,train.length);
    send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    // 发送文件内容
    while(1)
    {
        ssize_t sret = read(filefd,train.data,1000);
        train.length = sret;
        ssize_t sret1 = send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
        if(sret == 0 || sret1 == -1)
        {
            break;
        }
    }
    close(filefd);
    return 0;
}

// 2.0 mmap版
//int transFile(int netfd)
//{
//    char filename[] = "file1";
//    train_t train;
//    // 发送文件名
//    train.length = strlen(filename);
//    memcpy(train.data,filename,train.length);
//    send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
//    // 打开文件并发送
//    int filefd = open(filename,O_RDWR);
//    ERROR_CHECK(filefd,-1,"open in transFile");
//    // 发送文件长度
//    struct stat statbuf;
//    fstat(filefd,&statbuf);
//    train.length = sizeof(statbuf.st_size);
//    memcpy(train.data,&statbuf.st_size,train.length);
//    send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
//    // 发送文件内容
//    //char *p = (char *)mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
//    //ERROR_CHECK(p,MAP_FAILED,"mmap");
//    //send(netfd,p,statbuf.st_size,MSG_NOSIGNAL);
//    //munmap(p,statbuf.st_size);
//    printf("done!\n");
//    sendfile(netfd,filefd,NULL,statbuf.st_size);
//    close(filefd);
//    return 0;
//}

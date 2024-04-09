#include "head.h"

int sendfd(int sockfd, int fdtosend, int idx, int exitFlag)
{
    struct msghdr hdr;
    // NULL 0
    bzero(&hdr,sizeof(hdr));
    // 正文 0:子进程在workerArr中下标 1:退出标志
    struct iovec vec[2];
    vec[0].iov_base = &idx;
    vec[0].iov_len = sizeof(int);
    vec[1].iov_base = &exitFlag;
    vec[1].iov_len = sizeof(int);

    hdr.msg_iov = vec;
    hdr.msg_iovlen = 2;
    // 控制字段部分，记录要发送的内核数据结构
    struct cmsghdr *pcmsg = (struct cmsghdr *)malloc(CMSG_LEN(sizeof(int)));
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS;
    // 通过pcmsg得到data首地址，强转成int*，再解引用
    *(int *)CMSG_DATA(pcmsg) = fdtosend;

    hdr.msg_control = pcmsg;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    // flags 无用

    // sendmsg
    int ret = sendmsg(sockfd,&hdr,0);
    ERROR_CHECK(ret,-1,"sendmsg");
    return 0;
}
int recvfd(int sockfd, int *pfdtorecv, int *pidx, int *pexitFlag)
{
    struct msghdr hdr;
    // NULL 0
    bzero(&hdr,sizeof(hdr));
    // 正文 0:子进程在workerArr中下标 1:退出标志
    struct iovec vec[2];
    vec[0].iov_base = pidx;
    vec[0].iov_len = sizeof(int);
    vec[1].iov_base = pexitFlag;
    vec[1].iov_len = sizeof(int);

    hdr.msg_iov = vec;
    hdr.msg_iovlen = 2;
    // 控制字段部分，存储接收的内核数据结构
    struct cmsghdr *pcmsg = (struct cmsghdr *)malloc(CMSG_LEN(sizeof(int)));
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS;

    hdr.msg_control = pcmsg;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    // flags 无用

    // recvmsg
    int ret = recvmsg(sockfd,&hdr,0);
    ERROR_CHECK(ret,-1,"recvmsg");
    // 获取fdtorecv
    *pfdtorecv = *(int *)CMSG_DATA(pcmsg);
    printf("fdtorecv = %d\n",*pfdtorecv);
    return 0;
}


#include <54func.h>

int main(int argc, char *argv[])
{
    // ./hw2 "www.baidu.com"
    ARGS_CHECK(argc,2);
    struct hostent *entry = gethostbyname(argv[1]);
    if(entry == NULL)
    {
        herror("gethostbyname");
        exit(-1);
    }
    printf("%s:\n官方名：%s\n别名：\n",argv[1],entry->h_name);
    for(int i = 0; entry->h_aliases[i] != NULL; ++i)
    {
        printf("\t %s \n",entry->h_aliases[i]);
    }
    printf("IP:\n");
    for(int j = 0; entry->h_addr_list[j] != NULL; ++j)
    {
        char ip[1024] = {0};
        inet_ntop(entry->h_addrtype,entry->h_addr_list[j],ip,sizeof(ip));
        printf("\t %s \n",ip);
    }
    return 0;
}


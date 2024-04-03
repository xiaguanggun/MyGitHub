#include <54func.h>

//打印文件类型及权限
void print_mode(mode_t mode);
//打印用户名
void print_user(uid_t uid);
//打印组名
void print_group(gid_t gid);
//打印最后修改时间
void print_time(time_t mtime);

int main(int argc, char * argv[])
{
    // ./myls dir
    DIR *dirp;
    if(argc == 1)
    {
        dirp = opendir(".");
    }
    else if(argc == 2)
    {
        dirp = opendir(argv[1]);
        chdir(argv[1]);
    }
    else
    {
        printf("args error.\n");
        exit(-1);
    }
    ERROR_CHECK(dirp,NULL,"opendir");

    struct dirent *pdirent;
    struct stat statbuf;
    while((pdirent = readdir(dirp)) != NULL)
    {
        int ret = stat(pdirent->d_name,&statbuf);
        ERROR_CHECK(ret,-1,"stat");
        //打印文件类型及权限
        print_mode(statbuf.st_mode);
        //打印硬链接数
        printf(" %3ld ", statbuf.st_nlink);
        //打印用户名
        print_user(statbuf.st_uid);
        //打印组名
        print_group(statbuf.st_gid);
        //打印文件大小
        printf(" %8ld ",statbuf.st_size);
        //打印最后修改时间
        print_time(statbuf.st_mtime);
        //打印文件名
        printf(" %s\n", pdirent->d_name);
    }

    closedir(dirp);
    return 0;
}

//打印文件类型及权限
void print_mode(mode_t mode)
{
    switch (mode & S_IFMT) {
    case S_IFBLK:  printf("b"); break;
    case S_IFCHR:  printf("c"); break;
    case S_IFDIR:  printf("d"); break;
    case S_IFIFO:  printf("p"); break;
    case S_IFLNK:  printf("l"); break;
    case S_IFREG:  printf("-"); break;
    case S_IFSOCK: printf("s"); break;
    default:       printf("-"); break;
    }
    bool mode_arr[9] = {false};
    for(int i = 8; i >= 0; i--)
    {
        mode_arr[i] = mode & 1;
        mode >>= 1;
    }
    for(int i = 0; i < 9; i++)
    {
        switch(i % 3)
        {
        case 0:
            printf("%c",mode_arr[i]?'r':'-');
            break;
        case 1:
            printf("%c",mode_arr[i]?'w':'-');
            break;
        case 2:
            printf("%c",mode_arr[i]?'x':'-');
            break;
        }
    }
    printf(" ");
    return;
}
//打印用户名
void print_user(uid_t uid)
{
    printf(" %s ", getpwuid(uid)->pw_name);
}
//打印组名
void print_group(gid_t gid)
{
    printf(" %s ", getgrgid(gid)->gr_name);
}
//打印最后修改时间
void print_time(time_t mtime)
{
    struct tm *mtim = localtime(&mtime);
    ERROR_CHECK(mtim,NULL,"localtime");
    printf(" %4d年 %2d月 %2d日 %02d:%02d:%02d ",
           mtim->tm_year+1900,
           mtim->tm_mon+1,
           mtim->tm_mday,
           mtim->tm_hour,
           mtim->tm_min,
           mtim->tm_sec);
}

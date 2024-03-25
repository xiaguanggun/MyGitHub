#include <54func.h>

//递归访问目录项
void readdir_rec(DIR *dirp, int h);

int main(int argc, char * argv[])
{
    // ./mytree dir
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
    
    readdir_rec(dirp, 1);

    closedir(dirp);
    return 0;
}

void readdir_rec(DIR *dirp, int h)
{
    struct dirent *pdirent;
    while((pdirent = readdir(dirp)) != NULL)
    {
        if(strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name,"..") == 0)
        {
            continue;
        }
        for(int i = 0; i < h; i++)
        {
            printf("    ");
        }
        printf("%s\n", pdirent->d_name);
        //目录就递归打印，打印完记得重置当前工作目录
        if(pdirent->d_type == DT_DIR)
        {
            //先记录当前工作目录
            char *curr_cwd = getcwd(NULL,0);
            // 先打开子目录再切换cwd
            DIR * dirp2 = opendir(pdirent->d_name);
            ERROR_CHECK(dirp2,NULL,"opendir");
            char buf[1024] = {0};
            // dir1/dir2
            sprintf(buf,"%s%s%s",getcwd(NULL,0),"/",pdirent->d_name);
            chdir(buf);
            readdir_rec(dirp2, h+1);
            //重置当前工作目录
            chdir(curr_cwd);
        }
    }
}

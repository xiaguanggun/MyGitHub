#include <54func.h>

int main()
{
    DIR * dirp = opendir(".");
    ERROR_CHECK(dirp,NULL,"opendir");
    struct dirent *pdirent;
    while((pdirent = readdir(dirp)) != NULL)
    {
        printf("%s ",pdirent->d_name);
    }
    printf("\n");
    closedir(dirp);
    return 0;
}


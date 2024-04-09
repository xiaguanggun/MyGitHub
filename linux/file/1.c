#include <54func.h>

typedef struct
{
    int id;
    char name[25];
    float score;
} student;

int main(int argc, char * argv[])
{
    // ./LD07_1 file
    ARGS_CHECK(argc, 2);
    student s[3] = {
        {1,"xiaohong",77.5f},
        {2,"xiaohuang",90.0f},
        {3,"xgg",100.0f}
    };
    int fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC, 0666);
    ERROR_CHECK(fd, -1 ,"open_1");
    write(fd, &s, sizeof(s));
    lseek(fd, 0, SEEK_SET);
    student s2[3];
    ssize_t sret = read(fd, &s2, sizeof(s2));
    ERROR_CHECK(sret, -1, "read");
    for(int i = 0; i < 3; i++)
    {
        printf("s%d:%d(id) %s(name) %.1f(score)\n", i, s2[i].id, s2[i].name, s2[i].score);
    }
    close(fd);

    return 0;
}


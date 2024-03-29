#include <54func.h>

void handler(int signum)
{
    printf("Before signum = %d\n",signum);
    sleep(5);
    printf("After signum = %d\n",signum);
}

int main()
{
    signal(SIGINT, handler);
    while(1)
    {
        sleep(1);
    }
    return 0;
}


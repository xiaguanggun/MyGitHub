#include <54func.h>

int main()
{
    if(fork())
    {
        return 0;
    }
    else
    {
        while(1)
        {
            sleep(1);
        }
    }
    return 0;
}


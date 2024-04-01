#include <54func.h>

int main()
{
    int i = 0x12345678;
    char *p = (char *)&i;
    printf("*p = %x\n",*p);
    return 0;
}


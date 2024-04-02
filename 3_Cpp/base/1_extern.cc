#include <iostream>
namespace MySpace
{
int swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
    return 0;
}
}

#include <iostream>
using namespace std;

namespace MySpace
{
extern int swap(int &a, int &b);
}

int main()
{
    int a = 10;
    int b = 20;
    MySpace::swap(a,b);
    cout << "a = " << a << endl << "b = " << b << endl;
    return 0;
}


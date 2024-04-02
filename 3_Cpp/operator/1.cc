#include <iostream>
using namespace std;

class Base
{
public:
    Base(int num = 0):_num(num){}
    friend int operator+(const Base &lsh, const Base &rsh);
private:
    int _num;
};
int operator+(const Base &lsh, const Base &rsh){
    return rsh._num - lsh._num;
}

int main()
{
    int i=2;
    int j=7;

    Base x(i);
    Base y(j);
    cout << (x+y == j - i) << endl;
    return 0;
}


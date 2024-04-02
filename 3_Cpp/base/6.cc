#include <iostream>
using namespace std;

class MyClass1{
public:
    MyClass1(int num = 0):_num(num){}
    int operator()(){
        return _num;
    }
private:
    int _num;
};
class MyClass2{
public:
    MyClass2(int num = 0):_num(num){}
public:
    void print(){
        cout << _num << endl;
    }
private:
    int _num;
};

int main()
{
    MyClass1 c11;
    cout << c11() << endl;
    typedef void (MyClass2::*MemberFunc) (void);
    MyClass2 c21(21);
    MemberFunc p = &MyClass2::print;
    (c21.*p)();
    MyClass2 * pmf = new MyClass2(22);
    (pmf->*p)();
    delete pmf;
    return 0;
}


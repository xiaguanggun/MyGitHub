#include <iostream>
using namespace std;

class Base{
public:
    virtual void fun1(){
        cout << "Base::func1()" << endl;
    }
    virtual void fun2(){
        cout << "Base::func2()" << endl;
    }
private:
    long _base;
};

class Derived:
    public Base
{
public:
    virtual void fun1() override{
        cout << "Derived::func1()" << endl;
    }
    virtual void fun3(){
        cout << "Derived::func3()" << endl;
    }
private:
    long _derived;
};

int main()
{
    typedef void (*pFunc)(void);
    Derived d;
    Base * pbase = &d;
    long * pDerived = reinterpret_cast<long *>(pbase);
    long * pVtable = reinterpret_cast<long *>(pDerived[0]);
    pFunc pfunc1 = (pFunc)pVtable[0];
    pFunc pfunc2 = (pFunc)pVtable[1];
    pFunc pfunc3 = (pFunc)pVtable[2];
    pfunc1();
    pfunc2();
    pfunc3();
    return 0;
}


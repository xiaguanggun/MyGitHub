#include <iostream>
using namespace std;

class MyClass
{
public:
    MyClass(int i = 0)
        :_data(i)
    {
        cout << i << endl;
        cout << "构造函数!" << endl;
    }
    int destroyMyClass(){
        cout << "destory!" << endl;
        delete this;
        return 0;
    }
private:
    ~MyClass(){
        cout << "析构函数!" << endl;
    }
private:
    int _data;
};

int main()
{
    MyClass *p = new MyClass(1);
    p->destroyMyClass();
    p = nullptr;
    return 0;
}

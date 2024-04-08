#include <iostream>
using namespace std;

class MyClass{
public:
    static MyClass * getInstance(){
        if(_pInstance == nullptr){
            cout << "new" << endl;
            _pInstance = new MyClass();
        }
        return _pInstance;
    }
    //static void destory(){
    //    if(_pInstance){
    //        cout << "delete" << endl;
    //        delete _pInstance;
    //        _pInstance = nullptr;
    //    }
    //}
    void init(int num){
        if(_pInstance){
            _pInstance->_num = num;
        }
        else {
            cout << "No Instance!" << endl;
        }
    }
    void print(){
        if(_pInstance){
            cout << "num = " << _num << endl;
        }
        else {
            cout << "No Instance!" << endl;
        }
    }
private:
    class AutoRelease{
    public:
        AutoRelease(){}
        ~AutoRelease(){
            if(_pInstance){
                cout << "~AutoRelease" << endl;
                delete _pInstance;
                _pInstance = nullptr;
            }
        }
    };
    MyClass():_num(0){}
    ~MyClass(){}
    int _num;
    static MyClass * _pInstance;
    static AutoRelease _ar;
};
MyClass * MyClass::_pInstance = nullptr;
MyClass::AutoRelease MyClass::_ar;

int main()
{
    MyClass::getInstance();
    MyClass::getInstance()->print();
    MyClass::getInstance()->init(10);
    MyClass::getInstance()->print();

    return 0;
}


#include <iostream>
#include <cstring>
using namespace std;

// 模板函数
template <class T1, class T2>
T1 add(T1 t1,T2 t2){
    return t1 + t2;
}

template <>
const char * add<const char *>(const char * t1,const char * t2){
    char *ptemp = new char[strlen(t1)+strlen(t2)+1]();
    strcpy(ptemp,t1);
    strcat(ptemp,t2);
    return ptemp;
}

// 模板类
template<class T = int,int kCapacity = 10>
class Stack{
public:
    Stack():_top(0),_data(new T[kCapacity]()){}
    ~Stack();
    bool empty();
    bool full(){
        return _top == kCapacity;
    }
    void push(T data){
        if(!full()){
            _data[_top++] = data;
        }
    }
    void pop(){
        if(!empty()){
            --_top;
        }
    }
    T top(){
        if(!empty()){
            return _data[_top-1];
        }
        cout << "Stack is empty!" << endl;
        return T();
    }
private:
    int _top;
    T *_data;
};
template<class T,int kCapacity>
Stack<T,kCapacity>::~Stack(){
    if(_data){
        delete [] _data;
        _data = nullptr;
    }
}
template<class T,int kCapacity>
bool Stack<T,kCapacity>::empty(){
    return _top == 0;
}

int main()
{
    cout << add(1,1) << endl;
    cout << add(1.1,1.1) << endl;
    cout << add("hello,","world!") << endl;

    cout << "------------" << endl;
    Stack<string> stk;
    stk.push("hello,");
    cout << stk.top() << endl;
    stk.pop();
    cout << stk.top() << endl;
    return 0;
}


#include <iostream>
using namespace std;

class Stack {
public:
    Stack():size(0){}
public:
	bool empty(){	//判断栈是否为空
        return size == 0;
    }
	bool full(){	//判断栈是否已满
        return size == 10;
    }
	void push(int data){ //元素入栈
        if(size < 10){
            stackArr[size] = data;
            ++size;
        }
    }
	void pop(){     //元素出栈
        if(size > 0){ --size; }
    }
	int  top(){		//获取栈顶元素
        return stackArr[size-1];
    }
    void print(){
        cout << "bottom-->";
        for(int i = 0; i < size; ++i){
            cout << stackArr[i] << "-->";
        }
        cout << "top" << endl;
    }
private:
    int stackArr[10];
    int size;
};

int main()
{
    Stack stk;
    cout << stk.empty() << endl;
    cout << stk.full() << endl;
    for(int i = 0; i < 11; ++i){
        if(!stk.full()){
            stk.push(i);
        }
        stk.print();
    }
    cout << "top = " << stk.top() << endl;
    for(int i = 0; i < 11; ++i){
        if(!stk.empty()){
            stk.pop();
        }
        stk.print();
    }
    return 0;
}


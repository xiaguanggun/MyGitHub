#include <iostream>
using std::cout;
using std::endl;

class Point{
    Point(int x, int y):
        _ix(x),
        _iy(y)
    {
        cout << "private:构造函数:Point(int,int)" << endl;
    }
    ~Point(){
        cout << "private:析构函数:~Point()" << endl;
    }
    Point(const Point & rhs) = delete;
    Point & operator=(const Point& rhs) = delete;
public:
    static Point * getInstance(){
        if(_pInstance == nullptr){
            _pInstance = new Point(1,2);
        }
        return _pInstance;
    }
    static void destroy(){
        if(_pInstance){
            delete _pInstance;
            _pInstance = nullptr;
        }
    }

    void print(){
        cout << "(" << _ix << "," << _iy << ")" << endl;
    }
    void setPoint(int x, int y){
        if(_pInstance == nullptr){
            cout << "没有Point实例!" << endl;
            return;
        }
        _ix = x;
        _iy = y;
        cout << "setPoint(" << _ix << "," << _iy << ")" << endl;
    }

private:
    int _ix;
    int _iy;
    static Point * _pInstance;
};
Point * Point::_pInstance = nullptr;

int main(){
    Point::getInstance()->print();
    Point::getInstance()->setPoint(2,3);
    Point::getInstance()->print();
    Point::destroy();

    return 0;
}


#include <iostream>
#include <cmath>
using namespace std;

const double PI = atan(static_cast<double>(1.0)) * 4;

class Circle{
public:
    Circle(double r = 0):_r(r){}
    double getArea(){
        return PI * _r * _r;
    }
    double getPerimeter(){
        return PI * 2 * _r;
    }
    void show(){
        cout << "r:" << _r << endl
             << "area:" << getArea() << endl
             << "perimeter:" << getPerimeter() << endl;
    }
private:
    double _r;
};

int main()
{
    Circle c1(3.0);
    c1.show();
    return 0;
}


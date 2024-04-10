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

class Cylinder:
    public Circle
{
public:
    Cylinder(double r = 0.0,double h = 1.0):Circle(r),_h(h){}
    double getVolume(){
        return getArea()*_h;
    }
    void showVolume(){
        cout << "Volume:" << getVolume() << endl;
    }
private:
    double _h;
};

int main()
{
    Cylinder cy1(3.0,2.0);
    cy1.showVolume();
    return 0;
}


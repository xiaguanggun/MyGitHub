#include <iostream>
using namespace std;

/* template <class T> */
/* double add(T t1){ */
/*     cout << "T1" << endl; */
/*     return t1; */
/* } */

/* double add(double t1){ */
/*     cout << "add()" << endl; */
/*     return t1; */
/* } */

/* void add(){ */
/*     cout << "add()" << endl; */
/* } */

/* template <class T,class...Args> */
/* T add(T t1, Args...args){ */
/*     cout << "Args" << endl; */
/*     return t1 + add(args...); */
/* } */

int add(){
    cout << "add()" << endl;
    return 0;
}

template <class T,class...Args>
double add(T t1, Args...args){
    cout << "Args" << endl;
    return t1 + add(args...);
}

int main()
{
    cout << add(1.2,2,3.5,4,5.1) << endl;
    return 0;
}


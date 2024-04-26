#include <iostream>
#include "Vector.h"
using namespace std;

int main()
{
    Vector<int> my_vec;
    for(int i = 0; i < 10; ++i){
        my_vec.push_back(i);
    }
    for(int i = 0; i < 11; ++i){
        my_vec.pop_back();
    }
    return 0;
}


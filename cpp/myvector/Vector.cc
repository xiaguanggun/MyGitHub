#ifndef __Vector_CC__
#define __Vector_CC__

#include <iostream>
#include "Vector.h"
using namespace std;


// static assert
template<typename T>
allocator<T> Vector<T>::_alloc{allocator<T>()};

// construct + destruct
template<typename T>
Vector<T>::Vector():
    _start(nullptr),
    _finish(nullptr),
    _end_of_storage(nullptr)
{}
template<typename T>
Vector<T>::~Vector(){
    if(!_start){
        return;
    }
    while(_start != _finish){
        _alloc.destroy(--_finish);
    }
    _alloc.deallocate(_start,capacity());
}


// member function
template<typename T>
void Vector<T>::push_back(const T& val){
    if(size() == capacity()){
        reallocate();
    }
    _alloc.construct(_finish++,val);
}
template<typename T>
void Vector<T>::pop_back(){
    if(size() == 0){
        cout << "pop_back out of range!" << endl;
        return;
    }
    _alloc.destroy(--_finish);
}
template<typename T>
inline int Vector<T>::size() const{
    return _finish - _start;
}
template<typename T>
inline int Vector<T>::capacity() const{
    return _end_of_storage - _start;
}

// expand capacity
template<typename T>
void Vector<T>::reallocate(){
    // allocate
    int oldCapacity = capacity();
    int newCapacity = oldCapacity > 0 ? oldCapacity << 1 : 1;
    T *newStart = _alloc.allocate(newCapacity);
    uninitialized_copy(_start,_finish,newStart);
    // construct + destroy
    while(_start != _finish){
        _alloc.destroy(--_finish);
    }
    // deallocate
    if(_start){
        _alloc.deallocate(_start,oldCapacity);
    }
    // update pointer
    _start = newStart;
    _finish = newStart + oldCapacity;
    _end_of_storage = newStart + newCapacity;
}

#endif

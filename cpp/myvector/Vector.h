#ifndef __Vector_H__
#define __Vector_H__

#include <memory>

template<typename T>
class Vector
{
public:
    Vector();
    ~Vector();
    void push_back(const T &);
    void pop_back();
    int size() const;
    int capacity() const;
private:
    void reallocate();//重新分配内存,动态扩容要用的
private:
    static std::allocator<T> _alloc;
    T *_start;                 //指向数组中的第一个元素
    T *_finish;                //指向最后一个实际元素之后的那个元素
    T *_end_of_storage;        //指向数组本身之后的位置
};

#include "Vector.cc"

#endif

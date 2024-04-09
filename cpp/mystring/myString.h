#ifndef __myString__
#define __myString__

#include <iostream>

class String 
{
private:
    // 内部类
    class InClass{
    public:
        InClass(std::size_t index,String *ps);
        //~InClass();
        static void deleteSi();
        //InClass &operator=(char);
        //InClass &operator=(const InClass &);
        char &operator=(char);
        // 类型转换函数,返回一个副本,可以传参、读
        // ostream自动转换,不需要重载<<
        operator char();
        //friend std::ostream &operator<<(std::ostream &os, const InClass &si);
    private:
        std::size_t _index;
        String *_ps;
    };
//public:
    // 内部类友元函数
    //friend std::ostream &operator<<(std::ostream &os, const InClass &si);
public:
    // 构造 + 析构
    String();
    String(const char *);
    String(const String &);
    ~String();
    // 赋值运算符
    String &operator=(const String &);
    String &operator=(const char *);
    String &operator+=(const String &);
    String &operator+=(const char *);
    // []
    InClass operator[](std::size_t index);
    const char &operator[](std::size_t index) const;
    // 成员函数
    std::size_t size() const;
    const char* c_str() const;
    const std::size_t & getRefCount() const;
    // 比较运算符
    friend bool operator==(const String &, const String &);
    friend bool operator!=(const String &, const String &);
    friend bool operator<(const String &, const String &);
    friend bool operator>(const String &, const String &);
    friend bool operator<=(const String &, const String &);
    friend bool operator>=(const String &, const String &);
    // 输入输出运算符
    friend std::ostream &operator<<(std::ostream &os, const String &s);
    friend std::istream &operator>>(std::istream &is, String &s);
    // 加法拼接运算符
    friend String operator+(const String &, const String &);
    friend String operator+(const String &, const char *);
    friend String operator+(const char *, const String &);
private:
    std::size_t _size;
    char * _pstr;
    //InClass _si;
    static std::size_t krefcountsize;
private:
    // 辅助函数
    void initRefCount();
    void increseRefCount();
    void decreseRefCount();
    void release();
    char * malloc();
};

#endif

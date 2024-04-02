#ifndef __myString__
#define __myString__

#include <iostream>

class String 
{
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
    char &operator[](std::size_t index);
    const char &operator[](std::size_t index) const;
    // 成员函数
    std::size_t size() const;
    const char* c_str() const;
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
};
// 比较运算符
bool operator==(const String &, const String &);
bool operator!=(const String &, const String &);
bool operator<(const String &, const String &);
bool operator>(const String &, const String &);
bool operator<=(const String &, const String &);
bool operator>=(const String &, const String &);
// 输入输出运算符
std::ostream &operator<<(std::ostream &os, const String &s);
std::istream &operator>>(std::istream &is, String &s);
// 加法拼接运算符
String operator+(const String &, const String &);
String operator+(const String &, const char *);
String operator+(const char *, const String &);

#endif

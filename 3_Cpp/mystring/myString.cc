#include <cstring>
#include <vector>
#include "myString.h"
using namespace std;

// 构造 + 析构
String::String():_size(0),_pstr(new char[1]()){}
String::String(const char *cstr):
    _size(strlen(cstr)),
    _pstr(new char[_size+1]())
{
    memcpy(_pstr,cstr,_size);
}
String::String(const String &rhs):
    _size(rhs._size),
    _pstr(new char[_size+1]())
{
    memcpy(_pstr,rhs._pstr,_size);
}
String::~String(){
    if(_pstr){
        delete [] _pstr;
        _pstr = nullptr;
    }
}
// 赋值运算符
String & String::operator=(const String &rhs){
    // 自赋值
    if(&rhs == this){
        return *this;
    }
    _size = rhs._size;
    delete [] _pstr;
    _pstr = new char[_size+1]();
    memcpy(_pstr,rhs._pstr,_size);
    return *this;
}
String & String::operator=(const char *cstr){
    _size = strlen(cstr);
    delete [] _pstr;
    _pstr = new char[_size+1]();
    memcpy(_pstr,cstr,_size);
    return *this;
}
String & String::operator+=(const String &rhs){
    _size += rhs._size;
    char *temp = new char[_size+1]();
    strcpy(temp,_pstr);
    strcat(temp,rhs._pstr);
    delete [] _pstr;
    _pstr = temp;
    return *this;
}
String & String::operator+=(const char *cstr){
    _size += strlen(cstr);
    char *temp = new char[_size+1]();
    strcpy(temp,_pstr);
    strcat(temp,cstr);
    delete [] _pstr;
    _pstr = temp;
    return *this;
}
// []
char & String::operator[](std::size_t index){
    // 验证index
    if(index < 0 || index > _size){
        cout << "index out of range!" << endl;
        static char nullchar = '\0';
        return nullchar;
    }
    return _pstr[index];
}
const char & String::operator[](std::size_t index) const{
    // 验证index
    if(index < 0 || index > _size){
        cout << "index out of range!" << endl;
        static char nullchar = '\0';
        return nullchar;
    }
    return _pstr[index];
}
// 成员函数
std::size_t String::size() const{
    return _size;
}
const char* String::c_str() const{
    return _pstr;
}
// 比较运算符
bool operator==(const String &lhs, const String &rhs){
    return !strcmp(lhs._pstr,rhs._pstr);
}
bool operator!=(const String &lhs, const String &rhs){
    return strcmp(lhs._pstr,rhs._pstr);
}
bool operator<(const String &lhs, const String &rhs){
    return strcmp(lhs._pstr,rhs._pstr) < 0;
}
bool operator>(const String &lhs, const String &rhs){
    return operator<(rhs,lhs);
}
bool operator<=(const String &lhs, const String &rhs){
    return !operator>(lhs,rhs);
}
bool operator>=(const String &lhs, const String &rhs){
    return !operator<(lhs,rhs);
}
// 输入输出运算符
std::ostream & operator<<(std::ostream &os, const String &s){
    return os << s._pstr;
}
std::istream & operator>>(std::istream &is, String &s){
    // 使用动态数组
    vector<char>temp;
    char ch;
    while((ch = is.get()) != '\n'){
        temp.push_back(ch);
    }
    s._size = temp.size();
    delete [] s._pstr;
    s._pstr = new char[s._size+1]();
    memcpy(s._pstr,&temp[0],s._size);
    return is;
}
// 加法拼接运算符
String operator+(const String &lhs, const String &rhs){
    return String(lhs) += rhs;
}
String operator+(const String &lhs, const char *cstr){
    return String(lhs) += cstr;
}
String operator+(const char *cstr, const String &rhs){
    return String(cstr) += rhs;
}

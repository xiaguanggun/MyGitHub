#include <cstring>
#include <vector>
#include "myString.h"
using namespace std;

// 辅助函数
void String::initRefCount(){
    *(std::size_t *)(_pstr-krefcountsize) = 1;
}
void String::increseRefCount(){
    ++*(std::size_t *)(_pstr-krefcountsize);
}
void String::decreseRefCount(){
    --*(std::size_t *)(_pstr-krefcountsize);
}
void String::release(){
    if(getRefCount() == 1){
        delete [] (_pstr-krefcountsize);
        _pstr = nullptr;
    }
    else{
        decreseRefCount();
    }
}
char * String::malloc(){
    return (new char[_size+1+krefcountsize]() + krefcountsize);
}
// 内部类
String::InClass::InClass(std::size_t index,String *ps):_index(index),_ps(ps){}
//String::InClass::~InClass(){}
//String::InClass & String::InClass::operator=(char ch){
//    // 验证index
//    if((_index < 0) || _index > _ps->_size){
//        cout << "index out of range!" << endl;
//        static String::InClass nullInClase(0,new String());
//        return nullInClase;
//    }
//    if(_ps->getRefCount() > 1){
//        char *temp = _ps->malloc();
//        strcpy(temp,_ps->_pstr);
//        _ps->decreseRefCount();
//        _ps->_pstr = temp;
//        _ps->initRefCount();
//    }
//    _ps->_pstr[_index] = ch;
//    return *this;
//}
//String::InClass & String::InClass::operator=(const InClass &rhs){
//    // 验证index
//    if((_index < 0) || _index > _ps->_size){
//        cout << "index out of range!" << endl;
//        static String::InClass nullInClase(0,new String());
//        return nullInClase;
//    }
//    if(_ps->getRefCount() > 1){
//        char *temp = _ps->malloc();
//        strcpy(temp,_ps->_pstr);
//        _ps->decreseRefCount();
//        _ps->_pstr = temp;
//        _ps->initRefCount();
//    }
//    _ps->_pstr[_index] = rhs._ps->_pstr[rhs._index];
//    return *this;
//}
char & String::InClass::operator=(char ch){
    // 验证index
    if((_index < 0) || _index > _ps->_size){
        cout << "index out of range!" << endl;
        static char nullchar = '\0';
        return nullchar;
    }
    if(_ps->getRefCount() > 1){
        char *temp = _ps->malloc();
        strcpy(temp,_ps->_pstr);
        _ps->decreseRefCount();
        _ps->_pstr = temp;
        _ps->initRefCount();
    }
    return _ps->_pstr[_index] = ch;
}
String::InClass::operator char(){
    // 验证index
    if((_index < 0) || _index > _ps->_size){
        cout << "index out of range!" << endl;
    }
    //cout << "operator char" << endl;
    return _ps->_pstr[_index];
}
//std::ostream & operator<<
//    (std::ostream &os, const String::InClass &si)
//{
//    // 验证index
//    if((si._index < 0) || si._index > si._ps->_size){
//        cout << "index out of range!" << endl;
//    }
//    else{
//        os << si._ps->_pstr[si._index];
//    }
//    return os;
//}


// String类
// 静态变量
std::size_t String::krefcountsize = sizeof(std::size_t);
// 构造 + 析构
String::String():_size(0),_pstr(malloc())
{
    initRefCount();
}
String::String(const char *cstr):
    _size(strlen(cstr)),
    _pstr(malloc())
{
    memcpy(_pstr,cstr,_size);
    initRefCount();
}
String::String(const String &rhs):
    _size(rhs._size),
    _pstr(rhs._pstr)
{
    increseRefCount();
}
String::~String(){
    release();
}
// 赋值运算符
String & String::operator=(const String &rhs){
    // 自赋值
    if(&rhs == this){
        return *this;
    }
    _size = rhs._size;
    _pstr = rhs._pstr;
    increseRefCount();
    return *this;
}
String & String::operator=(const char *cstr){
    release();
    _size = strlen(cstr);
    _pstr = malloc();
    memcpy(_pstr,cstr,_size);
    initRefCount();
    return *this;
}
String & String::operator+=(const String &rhs){
    _size += rhs._size;
    char *temp = malloc();
    strcpy(temp,_pstr);
    strcat(temp,rhs._pstr);
    release();
    _pstr = temp;
    initRefCount();
    return *this;
}
String & String::operator+=(const char *cstr){
    _size += strlen(cstr);
    char *temp = malloc();
    strcpy(temp,_pstr);
    strcat(temp,cstr);
    release();
    _pstr = temp;
    initRefCount();
    return *this;
}
// []
String::InClass String::operator[](std::size_t index){
    // 使用内部类
    return InClass(index,this);
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
const std::size_t & String::getRefCount() const{
    return *(std::size_t *)(_pstr-krefcountsize);
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
    s.release();
    s._pstr = s.malloc();
    memcpy(s._pstr,&temp[0],s._size);
    s.initRefCount();
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

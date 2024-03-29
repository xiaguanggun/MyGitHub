#include <iostream>
#include <string.h>
using namespace std;

class String
{
public:
    String():_size(0),_pstr(new char[1]()){}
    String(const char *pstr):
        _size(strlen(pstr)),
        _pstr(new char[_size+1])
    {
        memcpy(_pstr,pstr,_size+1);
    }
	String(const String &rhs):
        _size(rhs._size),
        _pstr(new char[_size+1])
    {
        memcpy(_pstr,rhs._pstr,_size+1);
    }
    ~String()
    {
        delete [] _pstr;
        _pstr = nullptr;
    }
	String &operator=(const String &rhs){
        _size = rhs._size;
        _pstr = new char[_size+1];
        memcpy(_pstr,rhs._pstr,_size+1);
        return *this;
    }
    void print(){
        cout << _pstr << endl;
    }
    size_t length() const{
        return _size;
    }
    const char * c_str() const{
        return _pstr;
    }
private:
    size_t _size;
    char * _pstr;
};

int main()
{
	String str1;
	str1.print();
	cout << str1.length() << endl;

	String str2 = "Hello,world";
	String str3("wangdao");
	
	str2.print();		
	cout << str2.length() << endl;
	str3.print();	
	cout << str3.length() << endl;
	
	String str4 = str3;
	str4.print();
	cout << str4.length() << endl;
	
	str4 = str2;
	str4.print();
	cout << str4.length() << endl;
	
	return 0;
}


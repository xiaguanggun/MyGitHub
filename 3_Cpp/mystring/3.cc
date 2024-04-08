#include "myString.h"
using namespace std;

void print(String &s1,String &s2,String &s3){
    cout << "------" << endl;
    cout << "s1:" << s1 << endl;
    cout << "s1ref = " << s1.getRefCount() << endl;
    cout << "s2:" << s2 << endl;
    cout << "s2ref = " << s2.getRefCount() << endl;
    cout << "s3:" << s3 << endl;
    cout << "s3ref = " << s3.getRefCount() << endl;
}

int main()
{
    // 构造 + 析构
    cout << "======构造 + 析构======" << endl;
    String s1;
    String s2("hello");
    String s3(s2);
    //print(s1,s2,s3);
    // 赋值运算符
    cout << "======赋值运算符======" << endl;
    s1 = s2;
    s2 = ",world";
    //print(s1,s2,s3);
    s3 += s2;
    //print(s1,s2,s3);
    s1 += s1;
    //print(s1,s2,s3);
    s1 += ",world";
    print(s1,s2,s3);
    // []
    s1[0] = 'H';
    cout << "s1:" << s1 << endl;
    const String s4("hello,s4!");
    cout << "s4[0]:" << s4[0] << endl;
    // 成员函数
    cout << "s4.size():" << s4.size() << endl;
    cout << "s4.c_str():" << s4.c_str() << endl;
    // 比较运算符
    cout << "s1 == s2 ? " << (s1 == s2) << endl;
    cout << "s1 != s2 ? " << (s1 != s2) << endl;
    cout << "s1 < s2 ? " << (s1 < s2) << endl;
    cout << "s1 > s2 ? " << (s1 > s2) << endl;
    cout << "s1 <= s2 ? " << (s1 <= s2) << endl;
    cout << "s1 >= s2 ? " << (s1 >= s2) << endl;
    // 输入运算符
    cin >> s1;
    cout << "s1:" << s1 << endl;
    print(s1,s2,s3);
    // 加法拼接运算符
    cout << "s1 + s2:" << s1 + s2 << endl;
    cout << "s1 + cstr:" << s1 + "cstr" << endl;
    cout << "cstr + s2:" << "cstr" + s2 << endl;
    print(s1,s2,s3);
    return 0;
}


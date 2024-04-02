#include "myString.h"
using namespace std;

int main()
{
    // 构造 + 析构
    String s1;
    String s2("hello");
    String s3(s2);
    cout << "s1:" << s1 << endl;
    cout << "s2:" << s2 << endl;
    cout << "s3:" << s3 << endl;
    // 赋值运算符
    s1 = s2;
    cout << "s1:" << s1 << endl;
    s2 = ",world";
    cout << "s2:" << s2 << endl;
    s3 += s2;
    cout << "s3:" << s3 << endl;
    //s1 += s1;
    //cout << "s1+s1:" << s1 << endl;
    s1 += ",world";
    cout << "s1:" << s1 << endl;
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
    // 加法拼接运算符
    cout << "s1 + s2:" << s1 + s2 << endl;
    cout << "s1 + cstr:" << s1 + "cstr" << endl;
    cout << "cstr + s2:" << "cstr" + s2 << endl;
    return 0;
}


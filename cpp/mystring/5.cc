#include "String.h"
using namespace std;

void print(String &s1,String &s2,String &s3){
    cout << "------" << endl;
    cout << "s1:" << s1 << endl;
    cout << "s1ref = " << s1.getRefCount() << endl;
    cout << "s2:" << s2 << endl;
    cout << "s2ref = " << s2.getRefCount() << endl;
    cout << "s3:" << s3 << endl;
    cout << "s3ref = " << s3.getRefCount() << endl;
    cout << "------" << endl;
}

int main()
{
    // 构造 + 析构
    cout << "======构造 + 析构======" << endl;
    String s1;
    String s2("hello");
    String s3 = s2;
    print(s1,s2,s3);
    cout << "s2[0]:" << s2[0] << endl;
    print(s1,s2,s3);
    // []
    s2[0] = 'H';
    print(s1,s2,s3);
    s3[20] = 'H';
    cout << "s3[-1]:" << s3[-1] << endl;
    const String s4("hello,s4!");
    cout << "s4[-1]:" << s4[-1] << endl;
    // 连续赋值
    print(s1,s2,s3);
    s2[1] = s3[0] = 'E';
    cout << "s2[1]:" << s2[1] << endl;
    cout << "s3[0]:" << s3[0] << endl;
    print(s1,s2,s3);
    // 移动构造、赋值
    cout << "======移动构造赋值======" << endl;
    String s5 = "yidonggouzao";
    cout << "s5:" << s5 << endl;
    cout << "s5ref = " << s5.getRefCount() << endl;
    s1 = "yidongfuzhi";
    print(s1,s2,s3);
    return 0;
}


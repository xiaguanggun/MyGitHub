#include <iostream>
#include <string.h>
using namespace std;

class Student{
public:
    // 成员函数
    int printStuent()
    {
        cout << "姓名:" << this->_name << endl;
        cout << "ID  :" << this->_id << endl;
        cout << "age :" << this->_age << endl;
        return 0;
    }
    // setStudent
    int setStudent(int id,const char *name,int age)
    {
        this->_id = id;
        this->_age = age;
        delete [] _name;
        _name = new char[strlen(name)+1]();
        strcpy(_name,name);
        return 0;
    }
    // 构造函数
    Student(int id,const char *name,int age)
        :_id(id),_name(new char[strlen(name)+1]()),_age(age)
    {
        cout << "传参构造函数:Student(int,string,int)" << endl;
        strcpy(_name,name);
    }
    Student()
        :_id(0),_name(new char[1]()),_age(0)
    {
        cout << "默认构造函数:Student()" << endl;
    }
    // 拷贝构造函数
    Student(Student & rhs)
        :_id(rhs._id),_name(new char[strlen(rhs._name)+1]()),_age(rhs._age)
    {
        cout << "拷贝构造函数:Student(const Student &)" << endl;
        strcpy(_name,rhs._name);
    }
    // 析构函数
    ~Student()
    {
        if(this->_name != nullptr){
            delete[] this->_name;
            this->_name = nullptr;
        }
        cout << "析构函数:~Student()" <<endl;
    }
private:
    int _id;
    char *_name;
    int _age;
friend int printStuent(const Student &s1);
};

int printStuent(const Student &s1)
{
    cout << "姓名:" << s1._name << endl;
    cout << "ID  :" << s1._id << endl;
    cout << "age :" << s1._age << endl;
    return 0;
}

int main()
{
    Student s1;
    s1.printStuent();
    Student s2(1,"xgg",18);
    printStuent(s2);
    Student s3(s2);
    s3.printStuent();
    s3.setStudent(2,"xgs",25);
    s3.printStuent();
    s3.~Student();
    return 0;
}


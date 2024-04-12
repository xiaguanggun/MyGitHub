#include <iostream>
#include <cstring>
using namespace std;

class Person{
public:
    Person(const char *name,int age):
        _name(new char[strlen(name)+1]()),
        _age(age)
    {
        strcpy(_name,name);
    }
    Person(const Person & rhs):
        _name(new char[strlen(rhs._name)+1]()),
        _age(rhs._age)
    {
        strcpy(_name,rhs._name);
    }
    ~Person(){
        if(_name){
            delete [] _name;
            _name = nullptr;
        }
    }
    void display(){
        cout << "name:" << _name << endl;
        cout << "age:" << _age << endl;
    }
    Person & operator=(const Person &rhs){
        delete [] _name;
        _name = new char[strlen(rhs._name)+1]();
        strcpy(_name,rhs._name);
        _age = rhs._age;
        return *this;
    }
private:
    char * _name;
    int _age;
};

class Employee:
    public Person
{
public:
    Employee(const char * name,int age,const char * department,int salary):
        Person(name,age),
        _department(new char[strlen(department)+1]()),
        _salary(salary)
    {
        strcpy(_department,department);
    }
    Employee(const Employee & rhs):
        Person(rhs),
        _department(new char[strlen(rhs._department)+1]()),
        _salary(rhs._salary)
    {
        strcpy(_department,rhs._department);
    }
    ~Employee(){
        delete [] _department;
    }
    void display(){
        Person::display();
        cout << "department:" << _department << endl;
        cout << "salary:" << _salary << endl;
    }
    Person & operator=(const Employee &rhs){
        Person::operator=(rhs);
        delete [] _department;
        _department = new char[strlen(rhs._department)+1]();
        strcpy(_department,rhs._department);
        _salary = rhs._salary;
        return *this;
    }
private:
    char * _department;
    int _salary;
};

int main()
{
    Employee e1("小明",18,"美术部",7500);
    Employee e2("小红",33,"引擎部",15000);
    Employee e3(e1);
    Employee e4("小黄",21,"策划部",9000);
    e4 = e2;
    e1.display();
    e2.display();
    e3.display();
    e4.display();
    return 0;
}


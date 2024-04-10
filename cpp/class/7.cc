#include <iostream>
#include <string>
using namespace std;

class Person{
public:
    Person(const string &name,int age = 18):_name(name),_age(age){}
    void display(){
        cout << "name:" << _name << endl
             << "age: " << _age << endl;
    }
private:
    string _name;
    int _age;
};

class Employee:
    public Person
{
public:
    Employee(const string &name,int age,
             const string &department,int salary):
        Person(name,age),
        _department(department),
        _salary(salary)
    {
        ++_totalNum;
        _totalSalary += salary;
    }
    ~Employee(){
        --_totalNum;
        _totalSalary -= _salary;
    }
    void display(){
        Person::display();
        cout << "department:" << _department << endl
             << "salary: " << _salary << endl;
    }
    int getSalary(){
        return _salary;
    }
    static int aveSalary(){
        return _totalSalary/_totalNum;
    }
private:
    string _department;
    int _salary;
    static int _totalNum;
    static int _totalSalary;
};
int Employee::_totalNum = 0;
int Employee::_totalSalary = 0;

int main()
{
    Employee e1("小明",18,"美术部",7500);
    Employee e2("小红",33,"引擎部",15000);
    Employee e3("小黑",25,"动画部",8500);
    Employee e4("小黄",21,"策划部",9000);
    e1.display();
    e2.display();
    e3.display();
    e4.display();
    cout << "aveSalary:" << Employee::aveSalary() << endl; 
    return 0;
}


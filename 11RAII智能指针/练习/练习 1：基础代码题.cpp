// 写一个 Student 类，包含：

// int id;
// string name;

// 要求：

// 构造函数打印 "Student 构造"；
// 析构函数打印 "Student 析构"；
// 使用 make_unique<Student> 创建对象；
// 调用成员函数打印学生信息。
#include <iostream>
#include <memory>
#include <string>
using namespace std;
class Student
{
    private:
    int id;
    string name;
    public:
    Student(int id,string name):
    id(id),
    name(name){
       cout<<"Student 构造" <<"\n";
    }
    void print(){
        cout<<name<<" "<<id<<endl;
    }
    ~Student(){
        cout<<"Student 析构" <<"\n";
    }

};
int main()
{
    unique_ptr<Student> p1 = make_unique<Student>(1,"LiMing");
    p1->print();
    return 0;
}
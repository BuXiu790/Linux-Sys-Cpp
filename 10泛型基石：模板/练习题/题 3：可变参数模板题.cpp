// 实现：

// printAll(1, "hello", 3.14, 'A');

// 输出：

// 1 hello 3.14 A

// 要求：

// 用 C++17 折叠表达式实现
#include<iostream>
using namespace std;
template <typename ...Args>
void printAll(Args...args){
   ((cout<<args<<" "),...);
}
int main(){
    printAll(1, "hello", 3.14, 'A');
    return 0;
}
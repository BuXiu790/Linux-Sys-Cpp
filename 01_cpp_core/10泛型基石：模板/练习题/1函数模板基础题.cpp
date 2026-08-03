// 题 1：函数模板基础题

// 实现一个函数模板：

// template <typename T>
// T myMin(T a, T b);

// 要求：

// 支持 int
// 支持 double
// 支持 char
// 在 main 中测试三种类型
#include<iostream>
using namespace std;
template<typename T>
T mymin(T a,T b){
    return a < b? a: b;
}

int main(){
    int a = 1,b = 2;
    double a2 = 1.0 ,b2 = 2.0;
    char a3 = 'A', b3 = 'B';
    cout<<mymin(a,b)<<endl;
    cout<<mymin(a2,b2)<<endl;
    cout<<mymin(a3,b3)<<endl;
    return 0;
}

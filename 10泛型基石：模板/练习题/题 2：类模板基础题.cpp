// 题 2：类模板基础题

// 实现一个类模板：

// template <typename T>
// class Box;

// 要求：

// 可以存一个 T 类型对象
// 提供 set
// 提供 get
// 测试 Box<int> 和 Box<string>
#include<iostream>
using namespace std;
template<typename T>
class Box{
private:
    T data;
public:
Box(T A)
:
data(A)
{}
    void set(T value){
        data = value;
        return ;
    }
    T get(){
        cout<<data;
        return data;
    }
};

int main(){
    //两个不同类
    Box<int> intbox(100);
    intbox.get();

    Box<char> charbox('D');
    cout<<endl;
    charbox.get();
    cout<<endl;


    return 0;
}
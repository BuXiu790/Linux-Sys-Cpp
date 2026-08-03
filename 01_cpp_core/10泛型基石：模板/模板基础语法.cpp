#include<iostream>
#include<vector>
using namespace std;
//模板是编译期多态，没有运行时多态（类似虚函数）的虚表开销

//1函数模板
template <typename T>
//表示返回值、参数 a、参数 b 都是同一种类型 T。
T maxValue(T a, T b){
    return a > b ? a : b;
}


//2类模板
template<typename T>
class stack
{
private:
    vector<T> data;
public:
    void push(const T & value){
        data.push_back(value);
    }
};
//类模板实例化，可以显示指定类型
//Stack<int> intStack;
//Stack<string> strStack;
//类模板不是具体类型，而是生成具体类的模板。
//只有当指定模板参数之后，例如 Stack<int>，它才成为一个具体类型。



//非类型参数模板
//T 是类型参数，N 是非类型模板参数。
template<typename T, size_t N>
class MyArray {
private:
    T data[N];

public:
    T& operator[](size_t index) {
        return data[index];
    }

    size_t size() const {
        return N;
    }
};
//MyArray<int, 5>


//可变参数模板
//可变参数模板可以接收任意数量、任意类型的参数。
template <typename... Args>
//typename... Args，声明包
//这里是在告诉编译器：“我要定义一个模板，它接收的类型数量是不定的。”
void printAll(const Args&... args)
//它声明了函数参数 args 是一个“参数包”。
//...在这里的逻辑是：把传入的每一个参数，都分别套上 const & 这种引用格式，然后一起塞进 args 这个包里。
 {
    ((cout << args << " "), ...);
//这里的 ... 才是真正的“解包”。
//((cout << args << " ") 是你希望对包里每一个元素做的动作。
//, 是连接符。
//... 告诉编译器：“把包里的东西按上面的动作，一个接一个地‘炸’开，中间用逗号连接。”
    cout << endl;
}

//模板实例化
//如果你从来不调用它，编译器不会生成具体函数。
//模板实例化。

// 模板实例化带来两个结果：

// 第一，性能通常很好。

// 因为类型在编译期确定，编译器可以内联优化。

// 第二，可能导致代码膨胀。

// 如果你对很多类型都实例化一个复杂模板，最终二进制体积可能变大。

// 这就是 STL 很强但编译慢、报错长的原因之一。

int main(){

    return 0;
}
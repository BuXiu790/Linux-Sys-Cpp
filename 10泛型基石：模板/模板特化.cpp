#include <iostream>
#include <string>
using namespace std;
//类模板支持全特化和偏特化，
//函数模板支持全特化，但不支持偏特化。
//函数模板遇到类似需求时，通常通过函数重载解决。

//1类模板全特化
//通用模板
//所有的特化都必须基于一个“通用版本”。
template <typename T>
struct TypeName {
    static string name() {
        return "unknown type";
    }
};

//特化，对特定类型的特殊处理方式
template <>
struct TypeName<int> {
    static string name() {
        return "int";
    }
};

template <>
struct TypeName<double> {
    static string name() {
        return "double";
    }
};

template <>
struct TypeName<string> {
    static string name() {
        return "string";
    }
};


// 类模板偏特化
template <typename T>
struct IsPointer {
    static const bool value = false;
};

template <typename T>
//偏特化版本，只要类型长得像 T*，就匹配这个版本。
struct IsPointer<T*> {
    static const bool value = true;
};



//2函数模板全特化
template <typename T>
void func(T val) { std::cout << "通用函数" << std::endl; }

template <>
void func<int>(int val) { std::cout << "专供 int 的函数" << std::endl; }


// 用重载代替偏特化！
template <typename T>
void func(T val) { /* 通用版本 */ }

template <typename T>
void func(T* val) { /* 指针重载版本 */ }



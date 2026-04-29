#include <iostream>
#include <string>

// ==========================================
// 1. 普通函数定义
// ==========================================
void printHello(const std::string& name) {
    std::cout << "Hello, " << name << "!" << std::endl;
}

int add(int a, int b) {
    return a + b;
}

// ==========================================
// 2. 类及成员函数定义
// ==========================================
class Calculator {
public:
    int multiply(int a, int b) {
        return a * b;
    }
    
    void showResult(int result) const {
        std::cout << "Result is: " << result << std::endl;
    }
    
    // 静态成员函数（与普通函数无异）
    static int subtract(int a, int b) {
        return a - b;
    }
};

int main() {
    // ---------------------------------------------------------
    // 阶段一：普通函数指针
    // ---------------------------------------------------------
    std::cout << "--- 普通函数指针 ---" << std::endl;
    
    // 语法：返回值类型 (*指针变量名)(参数类型列表)
    void (*funcPtr1)(const std::string&) = &printHello; 
    // 注意：对于普通函数，& 可省略，写成 = printHello 也可以
    
    int (*funcPtr2)(int, int) = add;

    // 调用语法：(*指针变量名)(实参) 或 指针变量名(实参)
    (*funcPtr1)("Alice"); 
    std::cout << "10 + 5 = " << funcPtr2(10, 5) << std::endl;


    // ---------------------------------------------------------
    // 阶段二：静态成员函数指针 (和普通函数指针语法完全一样)
    // ---------------------------------------------------------
    std::cout << "\n--- 静态成员函数指针 ---" << std::endl;
    
    int (*staticMemFuncPtr)(int, int) = &Calculator::subtract;
    std::cout << "10 - 5 = " << staticMemFuncPtr(10, 5) << std::endl;


    // ---------------------------------------------------------
    // 阶段三：非静态成员函数指针
    // ---------------------------------------------------------
    std::cout << "\n--- 非静态成员函数指针 ---" << std::endl;
    
    // 语法：返回值类型 (类名::*指针变量名)(参数类型列表)
    int (Calculator::*memFuncPtr1)(int, int) = &Calculator::multiply;
    // 注意：对于成员函数指针，必须严格使用 &类名::函数名，不可省略 &
    
    void (Calculator::*memFuncPtr2)(int) const = &Calculator::showResult; 
    // 如果是 const 成员函数，指针声明末尾也必须带 const

    // 调用语法：必须依赖具体的对象来调用
    Calculator calc;                   // 创建对象实例
    Calculator* calcPtr = &calc;       // 对象指针

    // 通过对象实例调用：(对象.*指针变量名)(实参)
    int res1 = (calc.*memFuncPtr1)(10, 5); 
    
    // 通过对象指针调用：(对象指针->*指针变量名)(实参)
    (calcPtr->*memFuncPtr2)(res1);

    return 0;
}
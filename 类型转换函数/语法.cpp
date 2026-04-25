#include <iostream>
#include <iomanip>

// ==========================================
// 准备工作 1：用于演示多态和 dynamic_cast 的类
// ==========================================
class Animal {
public:
    // 必须有虚函数（开启多态），dynamic_cast 才能在运行时查户口
    virtual ~Animal() {} 
    virtual void speak() const { std::cout << "Animal sounds..." << std::endl; }
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Woof!" << std::endl; }
    void fetch() { std::cout << "Fetching the ball!" << std::endl; } // 狗的专属技能
};

// ==========================================
// 准备工作 2：用于演示自定义类型转换的类
// ==========================================
class Fraction {
private:
    int numerator;
    int denominator;

public:
    // 1. [向内转换] explicit 构造函数：禁止将 double 隐式打包成 Fraction
    explicit Fraction(double val) {
        numerator = static_cast<int>(val * 100);
        denominator = 100;
    }
    
    Fraction(int n, int d) : numerator(n), denominator(d) {}

    // 2. [向外转换] explicit 类型转换函数：禁止将 Fraction 隐式解包成 double
    explicit operator double() const {
        return static_cast<double>(numerator) / denominator;
    }
};

// ==========================================
// 主程序：四大模块复习
// ==========================================
int main() {
    std::cout << "=== 模块一：隐式转换陷阱 ===" << std::endl;
    int a = -1;
    unsigned int b = 1;
    // 编译器偷偷把 -1 当成了无符号数（即最大正数）
    if (a < b) std::cout << "-1 < 1 是对的" << std::endl;
    else std::cout << "危险！发生隐式转换，-1 被当成了无符号最大值，所以 -1 > 1" << std::endl;


    std::cout << "\n=== 模块二：四大显式转换工具 (Casts) ===" << std::endl;
    
    // 1. static_cast (日常主力：良性、安全的转换)
    double pi = 3.14159;
    int int_pi = static_cast<int>(pi);
    std::cout << "1. static_cast: " << pi << " 截断为 " << int_pi << std::endl;

    // 2. dynamic_cast (多态专属：安全的向下转换)
    Animal* my_pet = new Dog(); // 向上转换，天然安全
    Dog* real_dog = dynamic_cast<Dog*>(my_pet); // 向下转换，安检员出动
    if (real_dog) {
        std::cout << "2. dynamic_cast: 安检通过，确实是狗，去接飞盘！" << std::endl;
        real_dog->fetch();
    }

    // 3. const_cast (权限解除：撕下单向只读标签，仅限指针或引用)
    int score = 100;
    const int* read_only_ptr = &score;
    int* editable_ptr = const_cast<int*>(read_only_ptr);
    *editable_ptr = 99; // 顺着双向管道改回去
    std::cout << "3. const_cast: 只读锁解除，分数被修改为 " << score << std::endl;

    // 4. reinterpret_cast (底层重释：无视类型的狂野转换)
    long long fake_address = 0x7FFF1234;
    int* hardware_ptr = reinterpret_cast<int*>(fake_address);
    std::cout << "4. reinterpret_cast: 纯数字被强行解释成了内存地址 " << hardware_ptr << std::endl;


    std::cout << "\n=== 模块三：自定义类型的安全转换 ===" << std::endl;
    Fraction frac(3, 4);

    // double val = frac; // ❌ 编译报错：因为加了 explicit，禁止自动解包
    double val = static_cast<double>(frac); // ✅ 必须显式强转
    std::cout << "自定义类型转换: 3/4 被显式转换为 " << val << std::endl;

    // Fraction f2 = 3.14; // ❌ 编译报错：因为加了 explicit，禁止自动打包
    Fraction f2 = static_cast<Fraction>(3.14); // ✅ 必须显式强转

    // 清理内存
    delete my_pet;

    return 0;
}
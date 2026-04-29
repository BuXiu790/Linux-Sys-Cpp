/**
 * Pimpl 模式总结示例
 * 包含：嵌套类、前置声明、智能指针管理、拷贝与移动语义
 */

#include <iostream>
#include <memory>  // 必须包含智能指针头文件
#include <vector>

// ==========================================
// 1. 接口类定义 (通常在 .h 文件中)
// ==========================================
class Widget {
public:
    Widget();                          // 构造函数
    ~Widget();                         // 析构函数：必须在.cpp中定义

    // 为了支持拷贝和移动，显式处理“五法则”
    Widget(const Widget& other);                // 深拷贝构造
    Widget& operator=(const Widget& other);     // 深拷贝赋值
    Widget(Widget&& other) noexcept;            // 移动构造 (转移所有权)
    Widget& operator=(Widget&& other) noexcept; // 移动赋值

    void executeTask();                // 暴露给用户的公共接口

private:
    class Impl;                        // 【语法核心】嵌套实现类的前置声明
    std::unique_ptr<Impl> pImpl;       // 【用法核心】指向实现的唯一智能指针
};

// ==========================================
// 2. 实现类定义与成员函数实现 (通常在 .cpp 文件中)
// ==========================================

// 【实现细节】在.cpp中定义嵌套类，对外不可见
class Widget::Impl {
public:
    std::vector<int> internalData;     // 隐藏庞大的成员变量
    void doWork() { 
        std::cout << "Impl: 正在处理内部数据，大小为: " 
                  << internalData.size() << std::endl; 
    }
};

// 构造：在堆上创建实现对象
Widget::Widget() : pImpl(std::make_unique<Impl>()) {
    pImpl->internalData.push_back(100);
}

// 析构：在.cpp中定义，此时编译器已知Impl的大小，能安全释放 unique_ptr
Widget::~Widget() = default;

// 深拷贝实现：克隆一份完整的 Impl 数据
Widget::Widget(const Widget& other) 
    : pImpl(std::make_unique<Impl>(*other.pImpl)) {}

Widget& Widget::operator=(const Widget& other) {
    if (this != &other) {
        *pImpl = *other.pImpl; // 利用 Impl 默认的拷贝赋值
    }
    return *this;
}

// 移动语义：只需交换指针地址，性能极高 🚀
Widget::Widget(Widget&& other) noexcept = default;
Widget& Widget::operator=(Widget&& other) noexcept = default;

// 接口转发：外部类只负责点菜，内部类负责炒菜
void Widget::executeTask() {
    pImpl->doWork(); 
}

// ==========================================
// 3. 使用示例
// ==========================================
int main() {
    Widget w1;
    w1.executeTask();

    Widget w2 = w1;             // 触发深拷贝，产生独立的数据副本
    Widget w3 = std::move(w1);  // 触发移动，w1 被掏空，数据瞬间转给 w3

    w3.executeTask();
    return 0;
}
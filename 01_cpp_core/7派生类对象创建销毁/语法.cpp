/**
 * C++ 派生类对象创建与销毁（后端开发核心考点总结）
 * 包含：
 * 1. 物理生命周期（由内而外构造，由外而内析构）
 * 2. 成员初始化列表（优化性能与传参）
 * 3. 虚析构函数（防止多态场景下的内存/资源泄漏）
 */

#include <iostream>
#include <string>

using namespace std;

// ==========================================
// 1. 成员对象类 (例如：需要被派生类包含的日志组件)
// ==========================================
class Logger {
public:
    // 需要参数的构造函数
    Logger(const string& logPath) {
        cout << "  [构建] -> Logger(成员对象)：打开日志文件 " << logPath << endl;
    }
    ~Logger() {
        cout << "  [清理] -> Logger(成员对象)：关闭日志文件，刷入磁盘" << endl;
    }
};

// ==========================================
// 2. 基类 (系统的地基)
// ==========================================
class Base {
public:
    // 基类构造函数，需要参数
    Base(int id) {
        cout << "[构建] 1. Base(基类)：分配底层通用资源，ID = " << id << endl;
    }

    // 💣 【后端铁律】：作为多态使用的基类，析构函数必须是 virtual！
    // 否则 delete 基类指针时，派生类和成员对象的析构函数将被直接跳过，导致严重内存泄漏。
    virtual ~Base() {
        cout << "[清理] 3. Base(基类)：释放底层通用资源" << endl;
    }
};

// ==========================================
// 3. 派生类 (业务拓展层)
// ==========================================
class Derived : public Base {
private:
    Logger myLogger; // 派生类内部的成员对象

public:
    // 🧩 【核心语法】：成员初始化列表 ( : )
    // 作用：在派生类构造函数体 {} 执行前，将参数“路由”给基类和成员对象。
    // 顺序提醒：真正的初始化顺序与列表顺序无关，只与类中声明的顺序有关（先基类，后成员）。
    Derived(int baseId, const string& path) 
        : Base(baseId),        // 传参给基类
          myLogger(path)       // 传参给成员对象
    {
        cout << "[构建] 2. Derived(派生类)：完成特有业务初始化" << endl;
    }

    // override 关键字(C++11)显式表明重写了基类的虚函数，增加代码可读性和安全性
    ~Derived() override {
        cout << "[清理] 1. Derived(派生类)：清理特有业务资源" << endl;
    }
};

// ==========================================
// 测试：模拟后端实际运行场景
// ==========================================
int main() {
    cout << "===== 🚀 阶段一：多态创建（基类指针指向派生类） =====" << endl;
    // 构造顺序：Base -> Logger -> Derived
    Base* serverPtr = new Derived(8080, "/var/log/server.log");
    
    cout << "\n... 服务器处理海量请求中 ...\n" << endl;

    cout << "===== 💥 阶段二：多态销毁（触发动态路由清理） =====" << endl;
    // 析构顺序：Derived -> Logger -> Base (得益于 virtual ~Base)
    delete serverPtr; 
    serverPtr = nullptr; // 安全编程习惯：释放后置空

    return 0;
}
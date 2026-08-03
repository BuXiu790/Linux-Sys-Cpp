#include <iostream>

// ==========================================
// 1. 多继承与菱形危机 (虚继承解决二义性)
// ==========================================
class Device {
public:
    int device_id;
    // 含有至少一个虚函数是使用 dynamic_cast (RTTI) 的底层前提
    virtual ~Device() = default; 
};

// 使用 virtual 关键字进行虚继承，保证 Device 在最底层只有一份共享实例
class Printer : virtual public Device {
public:
    // 编译器会在这里隐式插入 vbptr (虚基表指针)
};

class Scanner : virtual public Device {
public:
    // 编译器也会在这里隐式插入 vbptr (虚基表指针)
};

// Copier 多继承自 Printer 和 Scanner (菱形结构的底端)
class Copier : public Printer, public Scanner {};

int main() {
    std::cout << "--- 1. 虚继承解决二义性 ---\n";
    Copier myCopier;
    
    // 因为使用了虚继承，底层只有一份 device_id，编译器不会报二义性错误
    myCopier.device_id = 1024; 
    std::cout << "Copier 的 device_id: " << myCopier.device_id << "\n\n";


    // ==========================================
    // 2. 向上转型与指针漂移 (Upcasting & Pointer Adjustment)
    // ==========================================
    std::cout << "--- 2. 指针漂移现象 ---\n";
    Copier* pCopier = &myCopier;
    
    // 安全的向上转型 (隐式转换)
    Printer* pPrinter = pCopier; 
    Scanner* pScanner = pCopier;

    // 打印指针内部存储的真实物理内存地址
    std::cout << "pCopier  地址: " << pCopier << "\n";
    std::cout << "pPrinter 地址: " << pPrinter << " (排在最前，通常与 pCopier 相同)\n";
    std::cout << "pScanner 地址: " << pScanner << " (发生指针漂移，编译器自动加上了偏移量)\n\n";

    // 比较被调整后的地址（编译器会做隐式类型处理，返回 true）
    if (pCopier == pScanner) {
        std::cout << "逻辑判断: pCopier == pScanner 返回 true (编译器介入了)\n\n";
    }


    // ==========================================
    // 3. 向下转型迷宫 (Downcasting: static_cast vs dynamic_cast)
    // ==========================================
    std::cout << "--- 3. 向下转型的安全性 ---\n";
    
    // 场景 A：基类指针真正指向的是派生类对象
    Scanner* valid_scanner = new Copier();
    
    // 场景 B：基类指针指向的就是纯基类对象
    Scanner* pure_scanner = new Scanner();

    // [ static_cast ]：只做编译期静态检查，强行偏移指针，后果自负
    Copier* dangerous_cast = static_cast<Copier*>(pure_scanner); 
    std::cout << "static_cast   转换纯基类: 编译成功，但指针危险 (若调用 Copier 独有方法会崩溃)!\n";

    // [ dynamic_cast ]：运行期 RTTI 检查真实身份
    Copier* safe_cast = dynamic_cast<Copier*>(pure_scanner);
    if (safe_cast == nullptr) {
        std::cout << "dynamic_cast  转换纯基类: 拒绝转换，返回 nullptr，成功拦截!\n";
    }

    // 清理内存
    delete valid_scanner;
    delete pure_scanner;

    return 0;
}
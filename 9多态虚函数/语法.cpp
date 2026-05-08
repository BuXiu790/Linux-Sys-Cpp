#include <iostream>

using namespace std;
//vptr -> clone() -> final 去虚化 -> 指针偏移 -> vbptr
// ==========================================
// 第一部分：多态基础、内存布局与生命周期陷阱
// ==========================================
class Base {
public:
    int base_val; // 4字节

    // 【考点 1：vptr与内存模型】
    // 只要有虚函数，编译器就会安插隐藏的虚表指针 (vptr)。
    // 64位系统下 sizeof(Base) = 16 (8字节 vptr + 4字节 int + 4字节内存对齐填充)
    Base(int val = 0) : base_val(val) {
        // 【考点 2：构造/析构期间多态失效】
        // 此时对象的 vptr 仍然指向当前 Base 的虚表，若在此调用虚函数，不会触发子类重写的版本。
    }

    // 【考点 3：虚析构函数的必要性】
    // 保证 delete 父类指针时，触发动态绑定，正确沿着继承链向下调用子类析构。
    virtual ~Base() {
        cout << "Base 析构" << endl;
    }

    virtual void show() const {
        cout << "调用 Base 的 show" << endl;
    }

    // 【考点 4：Clone 模式 (虚构造)】
    // 在只持有基类指针且不知具体类型时，让对象顺着 vptr 自己复制自己。
    virtual Base* clone() const = 0; 
};

class Derived : public Base {
public:
    int derived_val;

    Derived(int b_val, int d_val) : Base(b_val), derived_val(d_val) {}

    ~Derived() override {
        cout << "Derived 析构" << endl;
    }

    void show() const override {
        cout << "调用 Derived 的 show，子类特有值为 " << derived_val << endl;
    }

    Base* clone() const override {
        return new Derived(*this); // 默认拷贝构造
    }
};

// ==========================================
// 第二部分：性能优化与去虚化 (Devirtualization)
// ==========================================

// 【考点 5：final 关键字斩断继承链】
// 编译器明确知道不会再有子类重写，从而将运行时的“查 vtable 跳转”优化为编译时的“直接寻址(去虚化)”。
class FinalDerived final : public Derived {
public:
    using Derived::Derived; // 继承构造函数
    
    void show() const override final { 
        cout << "调用 FinalDerived 的 show (性能优化版本)" << endl; 
    }
};

// ==========================================
// 第三部分：多态环境测试与内存切片
// ==========================================

// 【考点 6：对象切片 (Object Slicing)】
// ⚠️ 按值传递时，子类对象被强行塞进父类空间，物理空间被切断，新 vptr 被重置为基类！
//报错
// void dangerSlice(Base obj) {
//     cout << "【危险】发生切片：";
//     obj.show(); // 永远只会打印 Base 的内容
// }

// ✅ 传指针或引用：不发生物理拷贝，vptr 完美发挥作用。
void safePolymorphism(const Base& obj) {
    cout << "【安全】多态生效：";
    obj.show();
}

// ==========================================
// 第四部分：复杂继承体系的内存魔法
// ==========================================

// 【考点 7：多重继承与指针偏移 (Pointer Adjustment)】
class Base1 { public: virtual void f1() {} };
class Base2 { public: virtual void f2() {} };
class MultiDerived : public Base1, public Base2 { };

void testMultipleInheritance() {
    MultiDerived* d = new MultiDerived();
    // MultiDerived 对象内部包含 2 个 vptr
    Base1* p1 = d; // 地址不变
    Base2* p2 = d; // 发生【指针偏移】，p2 指向内存中 Base2 子对象的起始位置
    delete d;
}

// 【考点 8：菱形继承与虚继承 (Virtual Inheritance)】
class SuperBase { public: int data; }; // 避免底层产生两份冗余数据

class VBase1 : virtual public SuperBase { };
class VBase2 : virtual public SuperBase { };
class VDerived : public VBase1, public VBase2 { };

void testDiamondInheritance() {
    VDerived d;
    // 虚继承底层魔法：对象内部增加虚基类指针 (vbptr)，指向虚基类表 (vbtable)。
    // vbtable 存储【偏移量 (Offset)】，CPU 查表加上当前地址，精准找到唯一的 SuperBase 实例。
    d.data = 10; // 无二义性报错
}

// ==========================================
// 测试入口
// ==========================================
int main() {
    cout << "--- 场景 1：多态对象的安全复制与生命周期 ---" << endl;
    Base* original_ptr = new Derived(10, 99);
    Base* cloned_ptr = original_ptr->clone(); 
    
    cout << "\n--- 场景 2：对象切片警示 ---" << endl;
    //dangerSlice(*cloned_ptr);      // 输出基类方法
    safePolymorphism(*cloned_ptr); // 输出子类方法

    cout << "\n--- 场景 3：虚析构函数验证 ---" << endl;
    delete original_ptr; // 准确调用 Derived 析构 -> Base 析构
    delete cloned_ptr;

    return 0;
}
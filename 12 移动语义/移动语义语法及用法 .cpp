#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
using namespace std;

class Buffer {
private:
    int* data;
    int size;

public:
    // 1. 普通构造：申请资源
    Buffer(int n = 0) : data(n ? new int[n] : nullptr), size(n) {
        for (int i = 0; i < size; i++) data[i] = i + 1;
        cout << "构造 Buffer(" << size << ")\n";
    }

    // 2. 析构函数：释放资源
    ~Buffer() {
        delete[] data;
        cout << "析构 Buffer\n";
    }

    // 3. 拷贝构造：深拷贝，重新申请资源
    Buffer(const Buffer& other)
        : data(other.size ? new int[other.size] : nullptr),
          size(other.size) {
        copy(other.data, other.data + size, data);
        cout << "拷贝构造\n";
    }

    // 4. 拷贝赋值：释放旧资源，深拷贝新资源
    Buffer& operator=(const Buffer& other) {
        cout << "拷贝赋值\n";

        if (this != &other) {
            int* newData = other.size ? new int[other.size] : nullptr;
            copy(other.data, other.data + other.size, newData);

            delete[] data;

            data = newData;
            size = other.size;
        }

        return *this;
    }

    // 5. 移动构造：接管资源，不重新申请内存
    Buffer(Buffer&& other) noexcept
        : data(other.data), size(other.size) {
        cout << "移动构造\n";

        // 移动后，源对象必须变成可析构状态
        other.data = nullptr;
        other.size = 0;
    }

    // 6. 移动赋值：释放旧资源，再接管新资源
    Buffer& operator=(Buffer&& other) noexcept {
        cout << "移动赋值\n";

        if (this != &other) {
            delete[] data;          // 先释放自己原来的资源

            data = other.data;      // 接管资源
            size = other.size;

            other.data = nullptr;   // 源对象置空，防止 double free
            other.size = 0;
        }

        return *this;
    }

    void print() const {
        if (data == nullptr) {
            cout << "empty\n";
            return;
        }

        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
};

// 7. 返回局部对象：不要写 return std::move(b)
Buffer createBuffer() {
    Buffer b(3);
    return b;   // 让编译器做 RVO / NRVO / 移动优化
}

// 8. 构造函数参数常见写法：值传递 + move 到成员
class User {
private:
    string name;

public:
    User(string n) : name(std::move(n)) {}

    void print() const {
        cout << "User name = " << name << endl;
    }
};

// 9. 右值引用变量本身是左值
void test(Buffer&& b) {
    // Buffer x = b;              // b 有名字，是左值，会调用拷贝构造
    Buffer x = std::move(b);      // 想继续移动，必须再次 std::move
}

int main() {
    cout << "===== 拷贝构造 =====\n";
    Buffer b1(5);
    Buffer b2 = b1;               // 左值，调用拷贝构造

    cout << "\n===== 移动构造 =====\n";
    Buffer b3 = std::move(b1);    // std::move 把左值转成右值引用
    b3.print();
    b1.print();                   // 移动后对象有效，但资源已空

    cout << "\n===== 拷贝赋值 =====\n";
    Buffer b4(2);
    b4 = b2;                      // 左值赋值，调用拷贝赋值

    cout << "\n===== 移动赋值 =====\n";
    Buffer b5(10);
    b5 = std::move(b2);           // 释放旧资源，接管 b2 的资源
    b5.print();
    b2.print();

    cout << "\n===== vector 中的移动 =====\n";
    vector<Buffer> vec;
    vec.reserve(3);               // 避免扩容干扰观察

    Buffer temp(4);
    vec.push_back(temp);          // 拷贝
    vec.push_back(std::move(temp)); // 移动
    vec.emplace_back(6);          // 直接在容器内部构造

    cout << "\n===== 返回局部对象 =====\n";
    Buffer b6 = createBuffer();

    cout << "\n===== 参数 move 到成员 =====\n";
    string s = "Tom";
    User u1(s);                   // s 是左值，先拷贝到参数 n，再移动到成员
    User u2("Alice");             // 临时对象，适合移动
    u1.print();
    u2.print();

    cout << "\n===== unique_ptr 类似思想 =====\n";
    // unique_ptr 不能拷贝，只能移动：
    // unique_ptr<int> p2 = p1;          // 错误
    // unique_ptr<int> p2 = std::move(p1); // 正确，所有权转移

    return 0;
}
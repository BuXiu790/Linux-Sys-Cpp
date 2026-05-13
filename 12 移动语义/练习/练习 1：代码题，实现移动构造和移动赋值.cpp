// 补全下面类的移动构造函数。

// #include <iostream>
// using namespace std;

// class Buffer {
// private:
//     int* data;
//     int size;

// public:
//     Buffer(int n) : data(new int[n]), size(n) {
//         for (int i = 0; i < size; i++) {
//             data[i] = i;
//         }
//     }

//     ~Buffer() {
//         delete[] data;
//     }

//     Buffer(const Buffer&) = delete;
//     Buffer& operator=(const Buffer&) = delete;

//     // TODO：实现移动构造函数

//     void print() const {
//         if (data == nullptr) {
//             cout << "empty\n";
//             return;
//         }

//         for (int i = 0; i < size; i++) {
//             cout << data[i] << " ";
//         }
//         cout << endl;
//     }
// };

// int main() {
//     Buffer b1(5);
//     Buffer b2 = std::move(b1);

//     b2.print();
//     b1.print();

//     return 0;
// }

// 要求：

// 移动后 b2 能正常打印
// b1 能安全析构
// 不能内存泄漏
// 不能 double free
#include <iostream>
using namespace std;

class Buffer {
private:
    int* data;
    int size;

public:
    Buffer(int n) : data(new int[n]), size(n) {
        for (int i = 0; i < size; i++) {
            data[i] = i;
        }
    }

    ~Buffer() {
        delete[] data;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    // TODO：实现移动构造函数

    Buffer(Buffer&& other) noexcept
    :
    data(other.data),
    size(other.size)
    {
        //移动构造
        other.data= nullptr;
        other.size = 0;

    }

    //移动赋值
    Buffer& operator=(Buffer&&other){
        if(this != &other){
        delete[] data;
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size=0;
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
int main() {
    Buffer b1(5);
    Buffer b2 = std::move(b1);

    b2.print();
    b1.print();

    Buffer a1(5);
    Buffer a2(3);

    a1 = std::move(a2);

    a1.print();
    a2.print();


    return 0;
}
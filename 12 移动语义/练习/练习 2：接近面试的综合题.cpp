// 设计一个简化版 Socket 类。

// 要求：

// 内部有一个 int fd
// 析构时如果 fd != -1，打印：
// close fd: x

// 不用真的调用 Linux close，先用 cout 模拟。

// 禁止拷贝构造和拷贝赋值
// 支持移动构造
// 支持移动赋值
// 移动后源对象的 fd 变成 -1
// 写 main 测试移动构造和移动赋值

// 参考使用方式：

// Socket s1(3);
// Socket s2 = std::move(s1);

// Socket s3(4);
// s3 = std::move(s2);
#include <iostream>
using namespace std;
class Socket
{
private:
    int fd;

public:
    Socket(int a) : fd(a)
    {
    }
    ~Socket()
    {
        if (fd != -1)
        {
            cout << "析构close fd: " <<fd<< endl;
        }
    }
    // 禁止拷贝构造和赋值
    Socket(const Socket &other) = delete;
    Socket &operator=(const Socket &other) = delete;

    // 移动构造
    Socket(Socket &&other) noexcept:
    fd(other.fd)
    {
        other.fd = -1;
    }
    // 移动赋值
    Socket &operator=(Socket &&other) noexcept
    {
        if (this != &other)
        {
            // 如果申请了空间要先释放
            //  这里没有申请堆空间
            if (fd != -1)
            {
                cout << "移动赋值close fd: " << fd << endl;
            }

            fd = other.fd;
            other.fd = -1;
        }
        return *this;
    }

    void print(){
        cout<<"print  fd: "<<fd<<endl;
    }
};
int main()
{
    Socket s1(3);
    Socket s2 = std::move(s1);//移动构造

    Socket s3(4);
    Socket ss(5);
    s3 = std::move(ss);//移动拷贝
    s1.print();
    s2.print();
    s3.print();
    return 0;
}
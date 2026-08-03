#include <memory>
#include <iostream>
using namespace std;
class User
{
public:
    User()
    {
        cout << "User 构造" << endl;
    }

    ~User()
    {
        cout << "User 析构" << endl;
    }

    void hello()
    {
        cout << "hello" << endl;
    }
};
// unique_ptr 最大特点：不能拷贝，只能移动。move
int main()
{
    // unique_ptr
    unique_ptr<User> p1(new User());

    // make_unique,返回unique_ptr<T>
    auto p2 = make_unique<User>();
    unique_ptr<User> p3 = make_unique<User>(); // 等价于创建一个 User 对象，并交给 unique_ptr<User> 管理。

    unique_ptr<int> p = make_unique<int>(100);
    // unique_ptr 的基本操作
    if (p)
    {
        cout << "p 管理着对象" << endl;
    }
    p.reset(); // 释放当前管理的对象，并让 p 变为空。
    if (!p)
    {
        cout << "p 已经为空" << endl;
    }

    // unique_ptr 管理数组
    unique_ptr<int[]> arr = make_unique<int[]>(5);
    for (int i = 0; i < 5; i++)
    {
        arr[i] = i * 10;
    }

    // release 和 reset
    // reset：释放旧资源，接管新资源
    // release：放弃管理权，但不释放资源

    //reset() 会释放当前资源，并可以接管新资源
    //；release() 只是放弃所有权并返回裸指针，不释放资源。
    //release() 后需要手动处理资源，否则会泄漏。
    p.reset(); // 释放当前管理的对象，并让 p 变为空。
    unique_ptr<int> p = make_unique<int>(10);
    int *raw = p.release(); // p 不再管理资源
    // ***raw 需要手动 delete

    //delete p.get();
    //p.get()只是拿到裸指针，资源所有权仍然属于 p。
    //double free



    // move移动语义
    unique_ptr<int> pp1 = make_unique<int>(10);
    // unique_ptr<int> p2 = p1; // 错误，不能拷贝
    unique_ptr<int> pp2 = move(pp1); // 把 pp1 管理的资源所有权转移给 pp2。
    // p1 == nullptr
    // p2 拥有资源
    move(pp1);
    // 不是移动资源本身，而是把 p1 转成右值，
    // 让 unique_ptr 的移动构造函数可以接管资源。
}

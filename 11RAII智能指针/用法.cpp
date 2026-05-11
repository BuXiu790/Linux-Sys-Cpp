#include <iostream>
#include <memory>
// 底层思想
template <typename T>
class UniquePtr
{
private:
    T *ptr;

public:
    explicit UniquePtr(T *p = nullptr) : ptr(p) {}

    ~UniquePtr()
    {
        delete ptr;
    }

    UniquePtr(const UniquePtr &) = delete;
    UniquePtr &operator=(const UniquePtr &) = delete;

    UniquePtr(UniquePtr &&other) noexcept : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    UniquePtr &operator=(UniquePtr &&other) noexcept
    {
        if (this != &other)
        {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T &operator*() const
    {
        return *ptr;
    }

    T *operator->() const
    {
        return ptr;
    }
};

// 用法
// unique_ptr 作为函数参数
//  场景 1：只使用，不接管所有权
using namespace std;

void printUser(const unique_ptr<int> &p) // 函数只是借用，不拥有资源
{
    if (p)
    {
        cout << *p << endl;
    }
}
// 场景 2：函数接管所有权
void consume(unique_ptr<int> p)
{
    cout << "consume: " << *p << endl;
}
int main()
{
    auto p = make_unique<int>(10);
    printUser(p); // 传引用

    consume(move(p)); // 把资源所有权交给 consume 函数。
    // 函数结束后，资源自动释放。

    return 0;
}

// 写：

// template <typename T, typename... Args>
// std::unique_ptr<T> createObject(Args&&... args);

// 要求能这样用：

// auto user = createObject<User>(1, "Tom");
#include<iostream>
#include<memory>
#include <string>
#include <utility>
using namespace std;
struct User{
    int data;
    string name;
    User(int a,string b):
    data(a),
    name(b){}
};
template <typename T, typename... Args>
unique_ptr<T> createObject(Args&&... args){
    return unique_ptr<T>(new T(forward<Args>(args)...));
    //最内层：... (参数包展开)
    //核心层：forward<Args>(args) (完美转发)
    //实例化层：new T(...) (动态内存分配与构造)
    //最外层：unique_ptr<T>(...) (RAII 资源接管)
}
int main(){
    auto user = createObject<User>(1, "Tom");
    return 0;
}
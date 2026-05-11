// 找出下面代码的问题并修改：

// #include <iostream>
// #include <memory>
// using namespace std;

// int main() {
//     unique_ptr<int> p = make_unique<int>(10);

//     int* raw = p.get();

//     delete raw;

//     cout << *p << endl;

//     return 0;
// }

// 你要回答：

// 哪里错？
// 为什么错？
// 改成正确代码。
// 面试官问 get() 能不能 delete，你怎么答？
#include <iostream>
#include <memory>
using namespace std;
int main() {
    unique_ptr<int> p = make_unique<int>(10);

    int* raw = p.get();

    

    cout << *p << endl;

    return 0;
}

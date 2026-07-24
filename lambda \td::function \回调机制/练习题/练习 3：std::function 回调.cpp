// 写一个函数：

// void calculate(int a, int b, function<void(int)> callback);

// 要求：

// 函数内部计算 a + b；
// 然后调用 callback，把结果传出去。

// 调用效果：

// calculate(3, 5, [](int result) {
//     cout << "result = " << result << endl;
// });



#include <iostream>
#include <functional>
using namespace std;
void calculate(int a, int b, function<void(int)> callback)
{
    int c = a + b;
    callback(c);
}
int main()
{

    calculate(3, 5, [](int result)
              { cout << result << endl; });

    return 0;


}
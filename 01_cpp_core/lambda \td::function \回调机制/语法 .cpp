#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;

// 1. 普通函数：可以被 std::function 保存
void normalFunc(int x) {
    cout << "normalFunc: " << x << endl;
}

// 2. 函数对象：本质是重载了 operator() 的类
struct Functor {
    void operator()(int x) const {
        cout << "Functor: " << x << endl;
    }
};

// 3. 用 std::function 实现回调函数
void calculate(int a, int b, function<void(int)> callback) {
    int result = a + b;

    if (callback) {          // std::function 调用前最好判断是否为空
        callback(result);
    }
}

// 4. 事件系统：后端里很常见的“注册回调 + 触发事件”模型
class Event {
public:
    void setCallback(function<void(int)> cb) {
        callback = cb;       // 保存回调
    }

    void trigger(int value) {
        if (callback) {
            callback(value);  // 触发事件时执行回调
        }
    }

private:
    function<void(int)> callback;
};

// 5. 模板接收 callable：比 std::function 更轻量，适合临时调用
template <typename Func>
void runTask(Func func) {
    func();
}

int main() {
    // 一、最基础 lambda
    auto hello = []() {
        cout << "Hello lambda" << endl;
    };

    hello();

    // 二、带参数和返回值
    auto add = [](int a, int b) {
        return a + b;
    };

    cout << "add = " << add(3, 5) << endl;

    // 三、显式指定返回值
    auto divide = [](double a, double b) -> double {
        return a / b;
    };

    cout << "divide = " << divide(10, 4) << endl;

    // 四、值捕获：捕获的是创建 lambda 那一刻的副本
    int x = 10;

    auto valueCapture = [x]() {
        cout << "valueCapture x = " << x << endl;
    };

    x = 20;
    valueCapture();          // 输出 10，不是 20

    // 五、引用捕获：捕获的是外部变量本身
    int y = 10;

    auto refCapture = [&y]() {
        cout << "refCapture y = " << y << endl;
    };

    y = 20;
    refCapture();            // 输出 20

    // 六、mutable：允许修改值捕获的副本，不影响外部变量
    int z = 10;

    auto mutableLambda = [z]() mutable {
        z++;
        cout << "inside z = " << z << endl;
    };

    mutableLambda();         // 输出 11
    cout << "outside z = " << z << endl;  // 仍然是 10

    // 七、捕获列表常见写法
    int a = 1;
    int b = 2;

    auto mixedCapture = [a, &b]() {
        // a 是值捕获，b 是引用捕获
        cout << "a = " << a << ", b = " << b << endl;
    };

    b = 100;
    mixedCapture();

    // 八、lambda 配合 sort
    vector<int> nums = {5, 1, 9, 3, 2};

    sort(nums.begin(), nums.end(), [](int lhs, int rhs) {
        return lhs > rhs;    // 降序：大的排前面
    });

    for (int n : nums) {
        cout << n << " ";
    }
    cout << endl;

    // 九、结构体排序
    struct Student {
        string name;
        int score;
    };

    vector<Student> students = {
        {"Tom", 90},
        {"Alice", 95},
        {"Bob", 90}
    };

    sort(students.begin(), students.end(),
        [](const Student& s1, const Student& s2) {
            if (s1.score != s2.score) {
                return s1.score > s2.score;   // 分数高的在前
            }
            return s1.name < s2.name;         // 分数相同，名字小的在前
        }
    );

    for (const auto& stu : students) {
        cout << stu.name << " " << stu.score << endl;
    }

    // 十、std::function 保存普通函数、lambda、函数对象
    function<void(int)> f;

    f = normalFunc;
    f(1);

    f = [](int x) {
        cout << "lambda in std::function: " << x << endl;
    };
    f(2);

    f = Functor();
    f(3);

    // 十一、std::function 为空时不能直接调用
    function<void()> emptyFunc;

    if (emptyFunc) {
        emptyFunc();
    } else {
        cout << "emptyFunc is empty" << endl;
    }

    // 十二、回调函数
    calculate(3, 5, [](int result) {
        cout << "calculate result = " << result << endl;
    });

    // 十三、事件系统
    Event event;

    event.setCallback([](int value) {
        cout << "event value = " << value << endl;
    });

    event.trigger(100);

    // 十四、move capture：把资源移动进 lambda
    auto ptr = make_unique<int>(999);

    auto moveCapture = [p = move(ptr)]() {
        cout << "moveCapture value = " << *p << endl;
    };

    moveCapture();

    // 此时 ptr 已经失去资源，不应该再解引用 ptr

    // 十五、模板接收 lambda：没有 std::function 的类型擦除开销
    runTask([]() {
        cout << "runTask with lambda" << endl;
    });

    // 十六、无捕获 lambda 可以转函数指针
    void (*funcPtr)() = []() {
        cout << "no capture lambda -> function pointer" << endl;
    };

    funcPtr();

    /*
        十七、重要坑：不要返回引用捕获的局部变量

        错误示例：

        function<void()> bad() {
            int count = 0;

            return [&count]() {
                cout << count << endl;
            };
        }

        原因：
        count 是局部变量，函数结束后销毁。
        lambda 保存的是引用，之后调用会产生悬空引用。

        正确思路：
        return [count]() mutable {
            cout << ++count << endl;
        };
    */

    /*
        十八、lambda 底层本质：

        auto f = [x](int y) {
            return x + y;
        };

        大致等价于：

        class LambdaObject {
        private:
            int x;

        public:
            LambdaObject(int x) : x(x) {}

            int operator()(int y) const {
                return x + y;
            }
        };

        捕获变量 -> 闭包对象的数据成员
        函数体   -> operator()
    */

    return 0;
}
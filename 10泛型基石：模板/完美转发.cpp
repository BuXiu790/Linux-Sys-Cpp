#include <iostream>
#include <utility>
using namespace std;

void process(int& x) {
    cout << "左值引用" << endl;
}

void process(int&& x) {
    cout << "右值引用" << endl;
}

//完美转发组合T&& + forward<T>()
template <typename T>
void wrapper(T&& value) {//T&& value利用引用折叠规则，既能接左值也能接右值
    //有名字的变量一律被视为左值，没有forward，value被视为左值
    process(std::forward<T>(value));//std::forward<T>：负责根据 T 里的存根，把参数“还原”后传下去：
}
    //wrapper(a);传入左值，T 会推导为 int&
    //wrapper(20);当传入右值：T 会推导为 int。


int main() {
    int a = 10;

    wrapper(a);
    wrapper(20);

    return 0;
}
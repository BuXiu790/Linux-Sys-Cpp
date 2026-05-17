// 写一个 lambda，接收两个 int，返回较大的那个数。

// 要求：

// auto maxValue = ...;
// cout << maxValue(3, 8) << endl;
#include<iostream>
using namespace std;
int main(){
    auto maxval = [](int a,int b){
        return max(a,b);
    };
    cout << maxval(3, 8) << endl;
}
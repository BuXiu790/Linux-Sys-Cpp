// 把下面代码改成安全写法：

// #include <iostream>
// using namespace std;

// int main() {
//     int* arr = new int[5];

//     for (int i = 0; i < 5; i++) {
//         arr[i] = i;
//     }

//     for (int i = 0; i < 5; i++) {
//         cout << arr[i] << endl;
//     }

//     delete[] arr;

//     return 0;
// }

// 要求：

// 使用 unique_ptr<int[]>；
// 使用 make_unique<int[]>；
// 不手动 delete[]。
#include<iostream>
#include<memory>
using namespace std;
int main(){
    unique_ptr<int[]> arr = make_unique<int[]>(5);
        for (int i = 0; i < 5; i++) {
        arr[i] = i;
    }
     for (int i = 0; i < 5; i++) {
        cout << arr[i] << endl;
    }

    return 0;
}
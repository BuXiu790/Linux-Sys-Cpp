// 实现一个简单事件系统：

// 要求写一个类：

// class Event {
// public:
//     void setCallback(function<void(int)> cb);
//     void trigger(int value);
// };

// 要求：

// setCallback 用来注册回调；
// trigger 用来触发事件；
// 如果没有设置回调，不要崩溃；
// 在 main 里注册一个 lambda；
// 调用 trigger(100) 后输出：
// event value = 100
#include <iostream>
#include <functional>
using namespace std;


// //lambda 是一段逻辑
// std::function 是保存这段逻辑的变量
// trigger 是未来某个时刻调用这段逻辑
class Event
{
public:
//用来设置回调函数
//在main函数设置回调函数
    void setCallback(function<void(int)> cb){
        callback = cb;
    };
    //调用回调函数
    void trigger(int value)
    {
       if(callback){
        callback(value);
       }
    }

private:
    function<void(int)> callback;
};

int main()
{
    Event e;
    e.setCallback([](int val){
        cout<<"event value ="<<val<<endl;
    });
    e.trigger(100);
return 0;
}
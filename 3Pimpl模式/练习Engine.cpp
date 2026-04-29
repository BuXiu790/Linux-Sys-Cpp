#include "练习Engine.h"
#include <iostream>
// 假设这里包含了非常多恶心的操作系统底层头文件，比如 <sys/epoll.h> 等

// ==========================================
// 你的任务 1：
// 完整地定义出 Impl 结构体，给它加一个 std::string 类型的变量用来存路径，
// 以及一个模拟的内部函数 void do_hard_work()。
//
// 你的任务 2：
// 实现 Engine 的构造函数、析构函数 和 start 方法。
//
// 灵魂拷问 (务必回答)：
// 为什么我非要把 `~Engine();` 声明在 .h 里，然后跑到 .cpp 里去写一个看起来毫无作用的空函数 `Engine::~Engine() = default;`？
// 如果我直接在 .h 文件里不写析构函数，或者顺手在 .h 里写成 `~Engine() {}`，编译器为什么会当场爆出极其诡异的错误？它到底在试图寻找什么？
// ==========================================
class Engine::Impl{
    public:
    void do_hard_work(){
    std::cout << "Engine is working hard with config: " << s << std::endl;
    }
    std::string s;
};
//构造函数，make智能指针pimpl_，指向Impl
Engine::Engine():
   pimpl_(std::make_unique<Impl>())
   {
   }

//析构函数直接default
Engine::~Engine() = default;

//start函数是对外暴露的接口，把config_path存入Impl，并调用do_hard_work()
void Engine::start(const std::string& config_path){
    pimpl_->s =  config_path;
    pimpl_->do_hard_work();    
}
int main(){
    Engine A;
    A.start("cyxnb");

}
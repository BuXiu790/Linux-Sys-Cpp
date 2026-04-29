#pragma once
#include <memory>
#include <string>

class Engine {
public:
    Engine();
    ~Engine(); // 🚨 注意这里：只有声明！

    // 禁用拷贝语义
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void start(const std::string& config_path);

private:
    class Impl; // 前置声明：真正的金库在这里
    std::unique_ptr<Impl> pimpl_; // 唯一私有成员：指向金库的钥匙
};
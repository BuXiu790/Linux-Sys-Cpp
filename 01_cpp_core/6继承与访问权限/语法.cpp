/**
 * =========================================================
 * 🚀 C++ 继承方式与访问权限核心笔记 (后端架构师视角)
 * =========================================================
 * 
 * 【权限推导核心法则：木桶效应与天花板】
 * 派生类中的最终权限 = min(基类原有权限, 继承方式)
 * 权限严格度：private > protected > public
 * 绝对禁区：基类的 private 成员，子类无论如何无法直接访问。
 */

//继承方式（你在类名冒号后面写的 public/private）限制的是外部调用者的视野。

//父类成员原本的访问权限（父类里面写的 public/protected/private）决定的是子类内部的视野。


#include <iostream>
#include <string>

// ==========================================
// 1. 基类：定义初始权限与抽象接口
// ==========================================
class NetworkClient {
private:
    int socket_fd;          // 绝对私有：亲儿子(子类)也不能直接碰

protected:
    // 家族提款机：子类可以通过这个合法渠道读取私有数据，但不能篡改
    int getSocketFd() const { return socket_fd; }
    
    void logError(const std::string& msg) {
        std::cout << "[内部日志] " << msg << "\n";
    }

public:
    // 虚析构函数：防止多态删除时(通过父类指针delete子类对象)发生内存泄漏！
    virtual ~NetworkClient() = default;

    // 纯虚函数：定义接口规范，强制子类必须实现
    virtual void sendRawBytes(const std::string& data) = 0; 
};

// ==========================================
// 2. Public 继承：表达 "Is-A" (是一个) 关系
// ==========================================
// 权限天花板最高，保持原样。常用于对外暴露多态接口。
class TCPClient : public NetworkClient {
public:
    void sendRawBytes(const std::string& data) override {
        // 子类内部可以访问父类的 protected 成员
        int fd = getSocketFd(); 
        logError("TCP 准备发送数据..."); 
        std::cout << ">>> 发送TCP字节流: " << data << "\n";
    }
};

class UDPClient : public NetworkClient {
public:
    void sendRawBytes(const std::string& data) override {
        std::cout << ">>> 发送UDP字节流: " << data << "\n";
    }
};

// ==========================================
// 3. Private 继承：表达 "Implemented-in-terms-of" (根据...实现)
// ==========================================
// 权限天花板极低，父类的 public/protected 到这里全变成 private。
// 作用：内部偷用父类功能，外部严防死守。
class LegacyRPC : private TCPClient {
public:
    void callRemote() {
        // 内部可以自由使用父类方法
        sendRawBytes("Legacy Data"); 
    }
    // 外部调用者拿不到 sendRawBytes()，因为被 private 继承隐藏了。
    // 缺点：编译期焊死，无法在运行时切换网络协议。
};

// ==========================================
// 4. 组合优于继承 (Strategy Pattern / 依赖注入)
// ==========================================
// 现代后端架构的最佳实践：运行时热插拔
class ModernRPC {
private:
    NetworkClient* client; // 通用钥匙：父类指针可以指向任何子类对象(向上转型)

public:
    ModernRPC(NetworkClient* initial_client) : client(initial_client) {}

    // 运行时瞬间切换底层实现，无需修改业务逻辑
    void setProtocol(NetworkClient* new_client) {
        client = new_client;
    }

    void callRemote(const std::string& method) {
        std::string payload = "RPC_Header_" + method;
        // 多态调用：通过隐藏的 vptr 查虚函数表(vtable)，精准调用具体子类的代码
        client->sendRawBytes(payload); 
    }
};

// ==========================================
// 5. 外部调用与测试 (main)
// ==========================================
int main() {
    TCPClient tcp;
    UDPClient udp;

    std::cout << "--- 正常业务期 ---\n";
    ModernRPC rpc(&tcp); // 注入 TCP 客户端
    rpc.callRemote("getUser");

    std::cout << "\n--- 流量洪峰期，动态切换UDP ---\n";
    rpc.setProtocol(&udp); // 依赖注入：热切换
    rpc.callRemote("getHotItem");

    // 错误示范警报：
    // tcp.socket_fd;          // Error! 父类 private 外部不可见
    // LegacyRPC legacy;
    // legacy.sendRawBytes("");// Error! private 继承导致接口被隐藏
    // NetworkClient* p = new NetworkClient(); // Error! 抽象类不可实例化

    return 0;
}
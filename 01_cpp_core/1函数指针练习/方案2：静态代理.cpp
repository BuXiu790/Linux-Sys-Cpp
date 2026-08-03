// ==== 底层 C 语言库的 API ====
// 定义一个函数指针类型：接收一个 int 型的文件描述符 fd
typedef void (*EventCallback)(int fd);

// 注册网络可读事件，当 fd 有数据时，底层会自动调用 cb(fd)
void register_read_event(int fd, EventCallback cb);
// =============================


// ==== 你的 C++ 业务层代码 ====
#include <iostream>
#include<unordered_map>
#include <unistd.h> // 用于 sleep 模拟延迟
void register_read_event(int fd, EventCallback cb) {
    std::cout << "[底层 C API] 已成功为 fd=" << fd << " 注册监听事件。" << std::endl;
    std::cout << "[底层 C API] 模拟网络数据到达..." << std::endl;
    
    // 模拟底层检测到数据，主动调用你传入的静态代理函数
    cb(fd); 
}
class TcpServer {
private:
    int server_fd_;
    static std::unordered_map<int,TcpServer*> mp;
public:
    //静态哈希表
    
    //构造函数，构造时填入哈希表
    TcpServer(int fd) : server_fd_(fd) {
        mp[server_fd_] = this;
    }
    ~TcpServer(){
        mp.erase(server_fd_);
        //析构时清理哈希表
    }
    //静态代理函数
    //依靠静态哈希表拿到this指针
    static void proxy_on_read_ready(int fd){
        auto it = mp.find(fd);
        if(it != mp.end()){
            TcpServer*  proxy_TcpServer = it->second;
            //拿到this指针，调用成员函数
            proxy_TcpServer->on_read_ready(fd);
        }else{
            std::cout<<"not found";
        }
    }
    

    // 这是你的成员函数，负责处理具体连接
    void on_read_ready(int client_fd) {
        std::cout << "Server (fd=" << server_fd_ << ") is handling client: " << client_fd << std::endl;
    }

    void start() {

        // register_read_event(server_fd_, &TcpServer::on_read_ready);
        
        //传入参数是不带this指针的静态成员函数地址，符合底层要求
        //用静态代理函数，拿到this指针，调用成员函数
        register_read_event(server_fd_, &TcpServer::proxy_on_read_ready);
    }
};
//类外初始化静态变量
 std::unordered_map<int,TcpServer*> TcpServer:: mp;
 int main() {
    TcpServer my_server(8080);
    my_server.start(); 
    return 0;
}
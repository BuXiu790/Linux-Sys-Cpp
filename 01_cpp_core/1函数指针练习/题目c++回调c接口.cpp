// // ==== 底层 C 语言库的 API ====
// // 定义一个函数指针类型：接收一个 int 型的文件描述符 fd
// typedef void (*EventCallback)(int fd);

// // 注册网络可读事件，当 fd 有数据时，底层会自动调用 cb(fd)
// void register_read_event(int fd, EventCallback cb);
// // =============================


// // ==== 你的 C++ 业务层代码 ====
// #include <iostream>

// class TcpServer {
// private:
//     int server_fd_;
// public:
//     TcpServer(int fd) : server_fd_(fd) {}

//     // 这是你的成员函数，负责处理具体连接
//     void on_read_ready(int client_fd) {
//         std::cout << "Server (fd=" << server_fd_ << ") is handling client: " << client_fd << std::endl;
//     }

//     void start() {
//         // 🚨 致命问题发生在这里：
//         // 你想把当前对象的 on_read_ready 注册到底层去。
//         // 但是下面的代码绝对编译不过：
//         // register_read_event(server_fd_, &TcpServer::on_read_ready);

       

//     }
// };



//方案一，把成员函数改为静态成员函数
 //同时把fd也设置为静态    
// ==== 底层 C 语言库的 API ====
// 定义一个函数指针类型：接收一个 int 型的文件描述符 fd
typedef void (*EventCallback)(int fd);

// 注册网络可读事件，当 fd 有数据时，底层会自动调用 cb(fd)
void register_read_event(int fd, EventCallback cb);
// =============================


// ==== 你的 C++ 业务层代码 ====
   
  
#include <iostream>

class TcpServer {
private:
    static int server_fd_;
public:
    TcpServer(int fd)  {}

    // 这是你的成员函数，负责处理具体连接
 
     void static on_read_ready(int client_fd) {
        std::cout << "Server (fd=" << server_fd_ << ") is handling client: " << client_fd << std::endl;
    }

    void start() {
        register_read_event(server_fd_, &TcpServer::on_read_ready);
    }
};
int TcpServer::server_fd_= 1;









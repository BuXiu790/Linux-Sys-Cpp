#include <iostream>
#include <string>
#include <string_view>

// 👁️ 1. 测试 string_view (零拷贝观察者)
// 相比于 void printString(const std::string& str)，
// string_view 不会在栈上构造 24 字节的临时对象，也绝对不拷贝底层字符数据。
void printString(std::string_view str) {
    std::cout << "接收到: " << str << " (长度: " << str.size() << ")\n";
}

int main() {
    std::cout << "--- 🚀 1. SSO (短字符串优化) 机制 ---\n";
    // 字符串较短，直接存储在 s1 对象本身的 24 字节栈内存中。
    // 底层那个被“偷”出来的标志位被设置为 0 (SSO 模式)。
    std::string s1 = "Hello"; 
    std::cout << "短字符串: " << s1 
              << ", 大小: " << s1.size() 
              << ", 容量: " << s1.capacity() << "\n\n";

    std::cout << "--- 🏢 2. 堆分配与动态扩容 ---\n";
    // 字符串过长，超出 SSO 限制（通常 15 字节）。
    // 24 字节内存瞬间“变身”：存放指向堆的指针、长度、和容量（标志位为 1）。
    std::string s2 = "This is a very long string to test heap allocation.";
    std::cout << "长字符串当前容量: " << s2.capacity() << "\n\n";

    std::cout << "--- 🛡️ 3. reserve() 避免迭代器失效 ---\n";
    std::string s3;
    // 提前“包下宴会厅”，直接向操作系统申请足够大的连续堆内存。
    s3.reserve(100); 
    std::cout << "reserve(100) 后的初始容量: " << s3.capacity() << "\n";
    
    // 此时追加大量字符，底层物理地址绝对不会变，完美避开扩容开销和野指针危险。
    for (int i = 0; i < 50; ++i) {
        s3 += 'a'; 
    }

    std::cout << "\n--- 🗜️ 4. shrink_to_fit() 释放空闲内存 ---\n";
    s3.clear(); // 清空内容，size 归零，但 capacity 不变，场地未退租
    std::cout << "clear() 后的容量 (未释放): " << s3.capacity() << "\n";
    
    s3.shrink_to_fit(); // 强制退租场地，将庞大的空闲内存真正还给操作系统
    std::cout << "shrink_to_fit() 后的容量: " << s3.capacity() << "\n\n";

    std::cout << "--- ⚡ 5. string_view 高效传参 ---\n";
    // 直接传入原生 C 风格字符串字面量，不会触发任何堆/栈的内存分配！
    printString("Super fast zero-copy string literal!");

    return 0;
}
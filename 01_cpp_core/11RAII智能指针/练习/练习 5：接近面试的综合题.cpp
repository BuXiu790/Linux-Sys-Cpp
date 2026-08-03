// 写一个简单的 RAII 文件管理类：
// class FileGuard
// 要求：
// 构造函数中打开文件；
// 析构函数中关闭文件；
// 禁止拷贝；
// 支持移动；
// 提供 write(const string& msg) 函数；
// 在 main 中测试它。
// 提示：
// 可以用 C 风格文件：
// FILE* fp;
// fopen
// fclose
// fprintf
// 你最终应该能解释：
// 为什么 FileGuard 不能拷贝，但可以移动？
// 这是非常接近面试的 RAII 题。
#include<iostream>
#include<memory>
using namespace std;
class FileGuard{
    private:
    FILE *file;
    public:
    //传入文件名和操作类型
    FileGuard(const char* filename,const char* mode){
        file = fopen(filename,mode);
        //打开文件
        cout<<"打开文件"<<endl;
    }
    ~FileGuard(){
        if(file != nullptr){
            fclose(file);
            cout<<"关闭文件"<<endl;
        }
        
        
    }
    
    //禁止拷贝
   FileGuard& operator=(const FileGuard& f) = delete;
   FileGuard(const FileGuard &other) = delete;

   //支持移动
   FileGuard(FileGuard &&other) noexcept{
    file = other.file;
    other.file = nullptr;
   }

   // 提供 write(const string& msg) 函数；
   void write(const string& msg){
    if(!file){
        return;
    }
    fprintf(file, "%s", msg.c_str());
   }

};
int main() {
    
    FileGuard fg1("test.txt", "w");
    fg1.write("Hello RAII!\n");

   
    FileGuard fg2(move(fg1)); 
    fg2.write("This is written by fg2.\n");
    

    cout << "\n准备结束，观察析构" << endl;
    return 0;
} 
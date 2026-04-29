
    // ==========================================
    // 你的任务：
    // 1. 补全解引用运算符 operator*
    // 2. 补全箭头运算符 operator->
    // 3. (致命问题)：为了防止有人写出 `UniqueGpuPtr<GpuTensor> p2 = p1;` 导致底层
    //    raw_ptr_ 被 p1 和 p2 析构两次引发 Core Dump，你该在类里加上哪几行代码来彻底封杀这种行为？
    // =========================================
#include<iostream>
using std::cout;
using std::endl;
struct GpuTensor {
    int id;
    void compute() {
         /* 模拟在GPU上计算 */
         cout<<"compute() "<<"id: "<<id<<endl;
         }
};

template <typename T>
class UniqueGpuPtr {
private:
    T* raw_ptr_; // 掌控 GPU 内存
public:
    // 假设构造函数已实现，负责分配显存
    explicit UniqueGpuPtr(T* ptr = nullptr) 
    : raw_ptr_(ptr)
     {}
     //重载解引用*
    T& operator*(){
        return *raw_ptr_;

    }
    //operator->会递归地调用那个对象的 operator->()，直到获取到一个原生指针为止
    T* operator->(){
        return raw_ptr_;
    }
    //防止有人写出 `UniqueGpuPtr<GpuTensor> p2 = p1
    UniqueGpuPtr(const UniqueGpuPtr & other) = delete;
    UniqueGpuPtr & operator=(const UniqueGpuPtr & A) = delete;

    // 假设析构函数已实现，负责释放显存
    ~UniqueGpuPtr() { if (raw_ptr_) {
        delete raw_ptr_;
        raw_ptr_ = nullptr;
     }}


};
void test1(){
    //构造智能指针A
    UniqueGpuPtr<GpuTensor> A(new GpuTensor);
    A->id = 1;
    A->compute();
   // UniqueGpuPtr<GpuTensor> B = A;
    UniqueGpuPtr<GpuTensor> B(new GpuTensor);
    B->id = 2;
    B->compute();


}
int main(){
    test1();
    return 0;
}
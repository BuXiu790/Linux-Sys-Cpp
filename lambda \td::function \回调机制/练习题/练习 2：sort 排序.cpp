// 给你一个结构体：

// struct Student {
//     string name;
//     int score;
// };

// 要求：

// 用 lambda 对 vector<Student> 排序。

// 规则：

// 分数高的排前面；
// 如果分数相同，名字字典序小的排前面。
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Student
{
    string name;
    int score;
};
int main()
{
    vector<Student> st = {
        {"Alice", 90},
        {"Bob", 95},
        {"Charlie", 90},
        {"David", 95}};
    sort(st.begin(), st.end(), [](const Student & a, const Student & b)
         {
             if (a.score == b.score)
             {
                 return a.name < b.name;
             }
             else
             {
                 return a.score > b.score;
             }
         });
    for (const auto &student : st)
    {
        cout << student.name << ": " << student.score << endl;
    }

    return 0;
}

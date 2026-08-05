# 📝 软件测试：断言（Assert）核心笔记

## 1. 🧠 什么是断言？

- **核心定义**：验证程序“实际运行结果（Actual）”与“预期结果（Expected）”是否一致的自动检查机制。
- **执行结果**：
  - 🟢 **True**：测试通过，继续执行。
  - 🔴 **False**：测试失败，抛出错误（如 `AssertionError`）或中断程序。

---

## 2. 🎯 断言类型与选择原则

### 相等断言（如 `assertEquals`）

- 验证 `Actual == Expected`。
- 💡 **优势**：失败时会自动输出具体差异，例如 `Expected: 80, Actual: 70`，信息明确，优先推荐使用。

### 真假断言（如 `assertTrue` / `assertFalse`）

- 验证布尔条件。
- ⚠️ **局限**：失败时通常只提示 `Expected true, got false`，不利于定位问题。

---

## 3. ⚡ C++ 中断言的应用

### ① C++ 标准库（`<cassert>`）

语法：

```cpp
assert(condition);
```

**特点**：条件为假时直接中断程序运行，常用于开发阶段捕获严重错误。

### ② GoogleTest（gtest）框架（重点 🌟）

在实际项目测试中，需要根据是否需要强制中断来选择宏：

| 断言类型 🛠️ | 示例 💡 | 失败后的行为 ⚡ | 适用场景 📌 |
| --- | --- | --- | --- |
| `EXPECT_*`（非致命） | `EXPECT_EQ(expected, actual);`<br>`EXPECT_TRUE(condition);` | 记录错误日志，但继续执行后续测试代码 | 相互独立的属性测试、非崩溃风险的检查 |
| `ASSERT_*`（致命） | `ASSERT_NE(ptr, nullptr);`<br>`ASSERT_FALSE(vec.empty());` | 记录错误日志，并立即中断当前测试函数 | 前置条件验证，例如防止空指针、数组越界引发段错误或崩溃 |

#### GoogleTest 常用断言速查

在实际的 C++ 项目测试中，我们更常用 GoogleTest 等专业框架。它的断言宏通常有两种形式：

- **`EXPECT_*`**：断言失败时记录错误，但继续执行后续代码。
- **`ASSERT_*`**：断言失败时立即中断当前测试函数。

| 需求 🎯 | `EXPECT` 形式（推荐） | `ASSERT` 形式（强中断） |
| --- | --- | --- |
| 相等判断 | `EXPECT_EQ(expected, actual);` | `ASSERT_EQ(expected, actual);` |
| 不等判断 | `EXPECT_NE(val1, val2);` | `ASSERT_NE(val1, val2);` |
| 真 / 假 | `EXPECT_TRUE(condition);` | `ASSERT_TRUE(condition);` |
| 字符串相等 | `EXPECT_STREQ(str1, str2);` | `ASSERT_STREQ(str1, str2);` |

# Debug 与 Release 学习笔记

## 1. 💡 什么是 Debug 与 Release？

在 C/C++ 项目中，Debug 和 Release 代表了程序编译时的两种不同状态。

### 🐛 Debug（开发/调试模式）

- **特点**：保留完整的代码信息，不做额外优化。
- **目的**：方便在开发过程中定位 Bug、跟踪变量变化。

### 🚀 Release（发布/交付模式）

- **特点**：对代码进行深度优化，去除所有不必要的调试信息。
- **目的**：生成体积最小、运行速度最快的最终可执行文件，用于交付给用户。

---

## 2. 📊 核心区别对比

| 特性 | 🐛 Debug 模式 | 🚀 Release 模式 |
| --- | --- | --- |
| 运行速度 | 较慢 | 极快 ⚡ |
| 文件体积 | 较大 📦 | 很小 🤏 |
| 代码优化 | 关闭优化（`-O0`） | 最高级优化（`-O3`） |
| 调试信息 | 包含（`-g`） | 去除 |
| 断言检查（`assert`） | 正常生效 | 自动被禁用（`-DNDEBUG`） |

---

## 3. ⚙️ 在 CMake 中设置构建类型

CMake 通过内置变量 `CMAKE_BUILD_TYPE` 来决定构建模式。

### ① 在命令行中指定模式

构建项目时，通过 `-DCMAKE_BUILD_TYPE` 参数直接传入。

切换为 Debug 模式：

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
```

切换为 Release 模式：

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

### ② 在 CMakeLists.txt 中设置默认保底模式

如果在命令行中完全不传递 `CMAKE_BUILD_TYPE`，CMake 不会自动指定模式，这可能导致编译器使用默认的非标准配置。

推荐最佳实践：在 `CMakeLists.txt` 中加入保底逻辑——若用户未手动传入，则默认设为 Debug：

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# 🛠️ 如果用户没有在命令行指定 CMAKE_BUILD_TYPE，则默认设置为 Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(my_app main.cpp)
```

---

## 🛠️ 总结提示

- **开发时**：使用 Debug，方便排查错误。
- **发布或性能测试时**：使用 Release，充分发挥硬件性能。

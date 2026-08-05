# 🎯 CMake Target 与属性作用域

## 1. 什么是 Target（目标）？

在现代 CMake 中，**Target 是构建系统的核心实体**。

Target 不仅代表最终产物，例如可执行程序或库文件，还封装了生成该产物所需的信息：

- 源文件
- 头文件搜索路径
- 依赖关系
- 编译选项
- 编译宏
- 需要链接的库

可以把 Target 理解为一个“带有完整构建说明的产物对象”。

```text
Target
├── 源文件
├── 头文件搜索路径
├── 编译选项
├── 依赖库
└── 最终产物
```

---

## 2. 四个核心指令

| 指令 | 语法格式 | 作用 |
| --- | --- | --- |
| `add_executable` 🚀 | `add_executable(目标名 源文件...)` | 定义一个可执行程序 Target |
| `add_library` 📦 | `add_library(目标名 [库类型] 源文件...)` | 定义一个库 Target |
| `target_include_directories` 📂 | `target_include_directories(目标名 作用域 目录...)` | 为指定 Target 添加头文件搜索目录 |
| `target_link_libraries` 🔗 | `target_link_libraries(使用者Target 作用域 被链接Target...)` | 将库链接到 Target，并建立构建和属性传递关系 |

### 2.1 `add_executable`：创建可执行程序

```cmake
add_executable(my_app main.cpp)
```

这里创建了名为 `my_app` 的可执行程序 Target，源文件是 `main.cpp`。

### 2.2 `add_library`：创建库

```cmake
add_library(render_engine STATIC renderer.cpp)
```

常见库类型：

| 类型 | 说明 | Linux 常见产物 |
| --- | --- | --- |
| `STATIC` | 静态库 | `.a` |
| `SHARED` | 动态库 | `.so` |
| `INTERFACE` | 纯接口库，不编译源文件 | 没有实体库文件 |

如果省略 `STATIC` 或 `SHARED`，CMake 会根据 `BUILD_SHARED_LIBS` 的设置决定生成静态库还是动态库。

### 2.3 `target_include_directories`：添加头文件搜索目录

```cmake
target_include_directories(render_engine PUBLIC include)
```

这表示 `render_engine` 可以在 `include` 目录中查找头文件。因为使用了 `PUBLIC`，依赖它的其他 Target 也会继承这个头文件搜索目录。

### 2.4 `target_link_libraries`：链接库 Target

```cmake
target_link_libraries(my_app PRIVATE render_engine)
```

这表示：

- `my_app` 需要链接 `render_engine`。
- CMake 会先构建 `render_engine`，再构建和链接 `my_app`。
- `my_app` 会继承 `render_engine` 对外公开的使用要求。

---

## 3. 三种属性作用域

作用域决定一个属性是只给当前 Target 使用，还是还要传递给依赖当前 Target 的其他 Target。

```text
                       ┌──────────────────────┐
                       │   当前 Target 的属性  │
                       └──────────┬───────────┘
                                  │
          ┌───────────────────────┼───────────────────────┐
          ▼                       ▼                       ▼
      PRIVATE                  PUBLIC                 INTERFACE
    🔒 仅自己使用          🌐 自己和依赖者使用       🔌 仅依赖者使用
      不传递                    会传递                 自己不使用
```

| 作用域 | 当前 Target 使用？ | 依赖当前 Target 的其他目标继承？ | 典型场景 |
| --- | --- | --- | --- |
| `PRIVATE` 🔒 | 是 ✅ | 否 ❌ | 只在内部 `.cpp` 中使用的头文件目录或依赖库 |
| `PUBLIC` 🌐 | 是 ✅ | 是 ✅ | 公共头文件需要的目录或依赖库 |
| `INTERFACE` 🔌 | 否 ❌ | 是 ✅ | 只需要传递给使用者的属性；常用于 Header-only 库 |

> 注意：这里的“当前 Target 不使用”描述的是属性传递规则。它不表示当前 Target 一定不参与编译。只有通过 `add_library(名称 INTERFACE)` 创建的接口库本身没有源文件编译产物。

---

## 4. 如何选择作用域？

可以按下面的顺序判断：

1. **只有当前 Target 的 `.cpp` 文件需要它吗？** 使用 `PRIVATE`。
2. **当前 Target 和使用者都需要它吗？** 使用 `PUBLIC`。
3. **当前 Target 自己不需要，只有使用者需要吗？** 使用 `INTERFACE`。

初学时可以先记住：

> **默认优先考虑 `PRIVATE`，确认属性需要向外传递时，再选择 `PUBLIC` 或 `INTERFACE`。**

---

## 5. 标准配置示例

项目结构示例：

```text
MyGameProject/
├── CMakeLists.txt
├── main.cpp
├── renderer.cpp
└── include/
    └── renderer.h
```

`CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyGameProject LANGUAGES CXX)

# 1. 创建静态库 Target
add_library(render_engine STATIC renderer.cpp)

# render_engine 自己和依赖它的 Target 都需要 include 目录
target_include_directories(render_engine PUBLIC include)

# 2. 创建可执行程序 Target
add_executable(my_cool_game main.cpp)

# 3. 将 render_engine 链接到 my_cool_game
target_link_libraries(my_cool_game PRIVATE render_engine)
```

依赖关系：

```text
my_cool_game
      │
      │ PRIVATE 链接
      ▼
render_engine
      │
      │ PUBLIC 公开
      ▼
include/ 头文件目录
```

`my_cool_game` 链接 `render_engine` 后，会自动继承它的 `PUBLIC` 头文件目录，因此 `main.cpp` 可以找到 `include/renderer.h`。

---

## 6. 核心速记

```text
add_executable              创建可执行程序 Target
add_library                 创建库 Target
target_include_directories  配置头文件搜索目录
target_link_libraries       链接库并建立依赖关系

PRIVATE    自己用，不传递
PUBLIC     自己用，也传递
INTERFACE  自己不用，只传递
```

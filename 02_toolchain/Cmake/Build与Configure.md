# 📌 CMake：Configure 与 Build

## 1. CMake 的定位

⚙️ **CMake 不是编译器，而是构建系统生成器（Build System Generator）。**

CMake 会读取项目中的 `CMakeLists.txt`，生成适合当前平台的构建文件，再调用实际的构建工具和编译器完成编译与链接。

```text
CMakeLists.txt
      ↓
Configure + Generate
      ↓
Makefile、Ninja 构建文件、Visual Studio 工程等
      ↓
Build
      ↓
可执行文件、静态库、动态库等
```

---

## 2. 两个核心阶段

| 对比项 | ⚙️ Configure / Generate（配置与生成） | 🛠️ Build（构建） |
| --- | --- | --- |
| 主要任务 | 检查系统环境、识别编译器、解析 `CMakeLists.txt`，生成原生构建文件 | 调用实际构建工具、编译器和链接器处理源码 |
| 推荐命令 | `cmake -S . -B build` | `cmake --build build` |
| 阶段产物 | `Makefile`、Ninja 构建文件、`.sln` 等 | 可执行文件、`.a`、`.so`、`.dll` 等最终目标文件 |

> `cmake -S . -B build` 这条命令通常会连续完成 **Configure** 和 **Generate**。学习初期可以把它们合称为“配置阶段”。

---

## 3. 配置命令解析

```bash
cmake -S . -B build
```

### `-S .`

- `-S` 用来指定源码目录。
- `.` 表示当前目录。
- 当前目录中通常包含项目的 `CMakeLists.txt`。

### `-B build`

- `-B` 用来指定构建目录。
- `build` 是存放生成文件和编译产物的目录。
- 这样可以让源码与构建产物分离，保持源码目录整洁。

这种方式称为**源外构建（Out-of-source Build）**。

---

## 4. 构建命令解析

```bash
cmake --build build
```

- `--build` 是 CMake 的模式标志，表示进入构建流程。
- 后面的 `build` 是构建目录路径。
- CMake 会根据该目录中已经生成的构建文件，调用对应的底层工具进行编译和链接。

可以这样理解：

```text
cmake --build build
      │         │
      │         └── 构建目录
      └──────────── 进入构建模式
```

---

## 5. 使用 `-D` 传递配置变量

配置阶段可以使用 `-D` 设置 CMake 变量：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

这里的 `CMAKE_BUILD_TYPE=Release` 表示使用 Release 构建类型。

> `CMAKE_BUILD_TYPE` 主要用于 Makefiles、Ninja 等单配置生成器。Visual Studio 等多配置生成器通常在构建时使用 `cmake --build build --config Release`。

---

## 6. 自动重新配置机制

### 只修改 `.cpp` 源文件

通常直接重新构建即可：

```bash
cmake --build build
```

### 修改 `CMakeLists.txt`

多数由 CMake 生成的构建系统会检测到变化，并在构建前自动重新配置，然后继续构建：

```bash
cmake --build build
```

如果自动重新配置没有发生，或者配置选项发生了变化，可以手动再次执行：

```bash
cmake -S . -B build
cmake --build build
```

---

## 7. 核心命令速记

```bash
# 第一次配置项目，生成构建文件
cmake -S . -B build

# 编译和链接项目
cmake --build build
```

一句话记忆：

> **Configure 决定“怎么构建”，Build 真正执行“开始构建”。**

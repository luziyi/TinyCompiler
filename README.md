# TinyCompiler (C--编译器)

一个用 C++实现的 C 语言子集(C--)编译器项目，包含词法分析器和语法分析器组件。

## 特性

- **词法分析**：基于有限状态机(FSM)实现，支持标识符、关键字、运算符和常量的识别
- **语法分析**：基于 LR 分析方法，包含 FIRST 集、FOLLOW 集计算和语法分析表的构建
- **错误检测**：能够检测并报告词法和语法错误
- **符号表管理**：实现了符号表的建立和查询功能

## 详细目录结构

```
TinyCompiler/
├── include/                     # 头文件目录
│   ├── lexical/                 # 词法分析器头文件
│   │   ├── FSM.h                # 有限状态机定义
│   │   ├── SymbolTable.h        # 符号表定义
│   │   ├── TokenCodes.h         # 词法单元类型定义
│   │   └── util.h               # 辅助函数
│   └── syntax/                  # 语法分析器头文件
│       ├── analysis.h           # 语法分析接口
│       ├── define.h             # 基本数据结构定义
│       ├── grammar.h            # 语法规则处理
│       ├── LRItemSet.h          # LR项目集定义
│       ├── LRTable.h            # LR分析表定义
│       └── SetCalculate.h       # First/Follow集计算
├── src/                         # 源代码目录
│   ├── CMakeLists.txt           # 主源码CMake配置
│   ├── main.cpp                 # 主程序入口
│   ├── lexical/                 # 词法分析相关源文件
│   │   ├── CMakeLists.txt       # 词法分析器CMake配置
│   │   ├── FSM.cpp              # 有限状态机实现
│   │   ├── lexical.cpp          # 词法分析主要实现
│   │   ├── SymbolTable.cpp      # 符号表实现
│   │   └── util.cpp             # 辅助函数实现
│   └── syntax/                  # 语法分析相关源文件
│       ├── CMakeLists.txt       # 语法分析器CMake配置
│       ├── analysis.cpp         # 语法分析实现
│       ├── grammar.cpp          # 语法规则处理实现
│       ├── LRItemSet.cpp        # LR项目集实现
│       ├── LRTable.cpp          # LR分析表实现
│       └── SetCalculate.cpp     # First/Follow集计算实现
│
├── data/                        # 测试数据和语法规则
│   ├── first.txt                # FIRST集输出结果
│   ├── follow.txt               # FOLLOW集输出结果
│   ├── grammar.txt              # 语法规则定义文件
│   ├── lexical.txt              # 词法分析结果
│   ├── symbolTable.txt          # 符号表输出
│   ├── action_table.csv         # LR分析表的 ACTION 部分
│   ├── goto_table.csv           # LR分析表的 GOTO 部分
│   ├── syntax.txt               # 语法分析结果
│   └── testsample/              # 测试用例目录
│       ├── test.sy              # 默认测试文件
│       ├── test-1.sy            # 测试文件1
│       ├── test-2.sy            # 测试文件2
│       └── test-3.sy            # 测试文件3
│
├── build/                       # 构建目录(自动生成)
│   ├── bin/                     # 可执行文件输出目录
│   │   └── tiny-compiler.exe    # 编译器可执行文件
│   ├── src/                     # 构建生成的中间文件
│   │   ├── lexical/             # 词法分析器构建结果
│   │   │   └── liblexical_lib.a # 词法分析静态库
│   │   └── syntax/              # 语法分析器构建结果
│   │       └── libsyntax_lib.a  # 语法分析静态库
│   ├── CMakeCache.txt           # CMake缓存文件
│   └── ...                      # 其他构建文件
│
├── docs/                        # 文档目录
│   ├── BUILD.md                 # 构建说明文档
│   └── 2025春季编译原理大作业.pdf # 作业要求文档
│
├── examples/                    # 示例代码目录
├── CMakeLists.txt               # 主CMake配置文件
├── LICENSE.md                   # 许可证文件
└── README.md                    # 本文件
```

## 运行方式

编译器支持以下几种运行方式：

```bash
# 1. 使用默认源文件和语法文件
./tiny-compiler

# 3. 同时指定源文件和语法文件
./tiny-compiler [源文件路径] [语法文件路径]
```

默认文件路径：

- 源文件：`../data/testsample/test.sy`
- 语法文件：`../data/grammar.txt`

## 编译构建

本项目使用 CMake 构建系统。请参考[构建说明](docs/BUILD.md)获取详细的构建步骤。

### 快速构建（Windows 环境）

```bash
# 创建并进入构建目录
mkdir build
cd build

# 配置项目
cmake ..

# 编译项目
cmake --build .

# 运行程序
cd bin
./tiny-compiler
```

## 编译器流程

编译器执行以下步骤：

1. **词法分析**

   - 读取源代码文件
   - 构造有限状态机进行词法单元识别
   - 生成词法单元序列和符号表
   - 输出结果到`data/lexical.txt`和`data/symbolTable.txt`

2. **语法分析**
   - 读取语法规则文件(`data/grammar.txt`)
   - 构建终结符和非终结符集合
   - 计算 FIRST 集和 FOLLOW 集
   - 生成项目集规范族
   - 构建语法分析表
   - 执行语法分析
   - 输出结果到`data/syntax.txt`、`data/first.txt`和`data/follow.txt`

## 支持的语言特性

C--语言支持以下特性：

- **数据类型**：int, float, void
- **控制结构**：if-else 语句
- **函数**：函数定义和调用
- **变量**：变量声明和赋值，包含全局变量局部变量以及常量的定义
- **运算符**：算术运算符、比较运算符和逻辑运算符
- **主函数**：支持 void main 和 int main 主函数定义

## 开发信息

- **编程语言**：C++ 11
- **构建工具**：CMake 3.10+
- **支持平台**：Windows, Linux, macOS

## 项目人员分工

| 姓名   | 学号       | 工作内容                                                             |
| ------ | ---------- | -------------------------------------------------------------------- |
| 武承霖 | 3022244338 | 实现 dfa 最小化，token 的定义与构建                                  |
| 王嘉仑 | 3022244358 | nfa 与 dfa 的实现，主扫描函数的实现                                  |
| 王鑫培 | 3022244362 | 实现 LR 项集构造与 LR 自动机，ACTION 表与 GOTO 表构建及加载          |
| 陆子毅 | 3022206045 | 文法处理与 FIRST/FOLLOW 集，实现 LR 项集构造与 LR 自动机             |
| 郑力墉 | 3022244344 | LR 语法分析主驱动与日志输出，ACTION 表与 GOTO 表构建及加载，撰写报告 |

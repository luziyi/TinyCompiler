# TinyCompiler (C--编译器)

一个用 C++实现的 C 语言子集(C--)编译器项目，包含词法分析器和语法分析器组件。

## 目录结构

```
TinyCompiler/
├── lex/                  # 词法分析器模块
│   ├── include/          # 词法分析器头文件
│   │   ├── FSM.h         # 有限状态机
│   │   ├── SymbolTable.h # 符号表
│   │   ├── TokenCodes.h  # 词法单元类型定义
│   │   └── util.h        # 实用工具函数
│   └── src/              # 词法分析器源文件
│       ├── FSM.cpp       # 有限状态机实现
│       ├── lexical.cpp   # 词法分析主要实现
│       ├── SymbolTable.cpp # 符号表实现
│       └── util.cpp      # 工具函数实现
├── syntax/               # 语法分析器模块
│   ├── include/          # 语法分析器头文件
│   │   ├── first_follow.h # First和Follow集合
│   │   ├── grammar.h     # 语法规则
│   │   ├── parser.h      # 语法解析器
│   │   ├── parser_table.h # 预测分析表
│   │   └── syntax.h      # 语法分析主接口
│   └── src/              # 语法分析器源文件
│       ├── first_follow.cpp # First和Follow集合实现
│       ├── grammar.cpp   # 语法规则实现
│       ├── parser.cpp    # 语法解析器实现
│       ├── parser_table.cpp # 预测分析表实现
│       └── syntax.cpp    # 语法分析主接口实现
├── data/                 # 测试和语法规则数据
│   ├── grammer.txt       # 语法规则定义文件
│   └── test.sy           # 测试源代码文件
├── result/               # 分析结果输出目录
│   ├── first.txt         # First集合输出
│   ├── follow.txt        # Follow集合输出
│   ├── lexical.txt       # 词法分析结果
│   ├── symbolTable.txt   # 符号表输出
│   ├── syntax_analysis.txt # 语法分析结果
│   └── table.txt         # 预测分析表输出
├── lib/                  # 生成的库文件
├── build/                # 构建目录（编译生成）
├── main.cpp              # 主程序入口
├── CMakeLists.txt        # 主编译配置
└── README.md             # 本文件
```

## 功能特性

1. 词法分析

   - 支持 C--语言的基本词法单元识别
   - 实现了有限状态机和符号表
   - 支持错误检测和报告

2. 语法分析
   - 实现 LL(1)文法分析
   - 支持 First 集合和 Follow 集合计算
   - 生成预测分析表
   - 执行语法分析并输出分析过程

## 编译方式

本项目使用 CMake 构建系统。按照以下步骤编译项目：

### 前置条件

- C++17 兼容的编译器
- CMake (版本 >= 3.10)

### 编译步骤

```bash
# 创建并进入构建目录
mkdir -p build
cd build

# 配置项目
cmake ..

# 编译项目
cmake --build .
# 或者直接使用
make
```

### 运行编译器

```bash
# 编译完成后，在项目根目录运行
./compiler [源文件路径]

# 如果不指定源文件路径，默认使用 data/test.sy
```

## 分析流程

编译器执行以下步骤：

1. 读取源代码文件
2. 执行词法分析，生成词法单元序列
3. 读取语法规则文件 (data/grammer.txt)
4. 构建 First 集合和 Follow 集合
5. 生成预测分析表
6. 执行语法分析
7. 输出分析结果到 result 目录

## 实现细节

### 词法分析器

词法分析器使用确定有限状态机(DFA)识别词法单元：

- 标识符和关键字
- 数字常量
- 运算符 (+, -, \*, /, %, =, >, <, ==, <=, >=, !=, &&, ||)
- 界符 ((, ), {, }, ;, ,)

所有识别的词法单元会输出到 result/lexical.txt 文件中。

### 语法分析器

语法分析器实现了 LL(1)预测分析方法：

1. 从语法规则文件读取文法
2. 构建非终结符和终结符集合
3. 计算 First 集合和 Follow 集合
4. 构建预测分析表
5. 使用分析表执行自顶向下的语法分析

分析结果输出到 result/syntax_analysis.txt 文件中。

## 开发环境

- 编程语言：C++
- 构建工具：CMake
- 支持平台：Windows/Linux

# TinyCompiler (C--编译器)

一个用C++实现的C语言子集(C--)编译器项目，包含词法分析器和语法分析器组件。

## 目录结构

```
TinyCompiler/
├── lex/                  # 词法分析器模块
│   ├── include/          # 词法分析器头文件
│   │   ├── Lexer.h       # 词法分析器主类
│   │   └── Token.h       # 词法单元类和类型定义
│   ├── src/              # 词法分析器源文件
│   ├── test/             # 词法分析器测试
│   └── CMakeLists.txt    # 词法分析器编译配置
├── syntax/               # 语法分析器模块
│   ├── include/          # 语法分析器头文件
│   ├── src/              # 语法分析器源文件
│   └── test/             # 语法分析器测试
├── test/                 # 集成测试
├── docs/                 # 项目文档
├── build/                # 构建目录（自动生成）
├── main.cpp              # 主程序入口
├── CMakeLists.txt        # 主编译配置
├── LICENSE.md            # 许可证
└── README.md             # 本文件
```

## 功能特性

1. 词法分析
   - 支持C--语言的基本词法单元识别
   - 实现了自动机和符号表
   - 支持错误检测和报告

2. 语法分析
   - 实现LL(1)文法分析
   - 支持基本的语法结构解析
   - 生成语法分析树

## 编译方式

本项目使用CMake构建系统。按照以下步骤编译项目：

### 前置条件
- C++17兼容的编译器
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

# 运行编译器
./bin/compiler <输入文件路径>
```

### 运行测试

```bash
cd build
ctest
```

## Lex 模块接口

Lex模块提供词法分析功能，将源代码转换为词法单元（Token）序列。

### 主要类

#### Token 类 (Token.h)

Token类表示源代码中的词法单元。

```cpp
class Token {
public:
    Token(TokenType type, const std::string& lexeme, int line);
    
    TokenType getType() const;           // 获取token类型
    const std::string& getLexeme() const; // 获取token的词素
    int getLine() const;                 // 获取token所在行号
    std::string toString() const;        // 将token转换为字符串（调试用）
};
```

支持的Token类型包括：
- 关键字：CONST, INT, FLOAT, VOID, IF, ELSE, RETURN
- 标识符：IDENTIFIER
- 常量：INT_CONST, FLOAT_CONST
- 运算符：PLUS, MINUS, MULTIPLY, DIVIDE, MOD, ASSIGN, EQ, NEQ, LT, GT, LE, GE, AND, OR, NOT
- 分隔符：LPAREN, RPAREN, LBRACE, RBRACE, SEMICOLON, COMMA
- 特殊token：EOF_TOKEN, ERROR

#### Lexer 类 (Lexer.h)

Lexer类负责将源代码分解为Token序列。

```cpp
class Lexer {
public:
    explicit Lexer(const std::string& source);  // 从源代码创建词法分析器
    
    Token nextToken();                      // 获取下一个token并前进
    Token peekToken();                      // 预览下一个token但不移动指针
    std::vector<Token> getAllTokens();      // 获取所有token
};
```

### 使用示例

```cpp
#include "lex/include/Lexer.h"
#include <iostream>

void tokenizeSource(const std::string& source) {
    tiny_compiler::Lexer lexer(source);
    
    // 逐个处理Token
    tiny_compiler::Token token = lexer.nextToken();
    while (token.getType() != tiny_compiler::TokenType::EOF_TOKEN) {
        std::cout << token.toString() << std::endl;
        token = lexer.nextToken();
    }
    
    // 或者一次性获取所有Token
    // std::vector<tiny_compiler::Token> tokens = lexer.getAllTokens();
}
```

## 开发环境

- 编程语言：C++ 17
- 构建工具：CMake 3.10+
- 测试框架：Google Test
- 支持平台：Windows/Linux

## 贡献者

- 词法分析模块开发
- 语法分析模块开发
- 文档和测试

## 参考资料

1. 《编译原理》(龙书)
2. C--语言规范文档

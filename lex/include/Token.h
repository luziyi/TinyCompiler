#pragma once
#include <string>
#include <unordered_map>

namespace tiny_compiler {

// Token类型枚举
enum class TokenType {
    // 关键字
    CONST,   // const
    INT,     // int
    FLOAT,   // float
    VOID,    // void
    IF,      // if
    ELSE,    // else
    RETURN,  // return

    // 标识符
    IDENTIFIER,  // 变量名、函数名等

    // 常量
    INT_CONST,    // 整数常量
    FLOAT_CONST,  // 浮点数常量

    // 运算符
    PLUS,      // +
    MINUS,     // -
    MULTIPLY,  // *
    DIVIDE,    // /
    MOD,       // %
    ASSIGN,    // =
    EQ,        // ==
    NEQ,       // !=
    LT,        // <
    GT,        // >
    LE,        // <=
    GE,        // >=
    AND,       // &&
    OR,        // ||
    NOT,       // !

    // 分隔符
    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }
    SEMICOLON,  // ;
    COMMA,      // ,

    // 特殊token
    EOF_TOKEN,  // 文件结束
    ERROR       // 错误token
};

class Token {
   public:
    Token(TokenType type, const std::string& lexeme, int line);

    // 获取token类型
    TokenType getType() const;

    // 获取token的词素
    const std::string& getLexeme() const;

    // 获取token所在行号
    int getLine() const;

    // 将token转换为字符串（用于调试）
    std::string toString() const;

    // 符号表相关
    bool isKeyword() const;
    bool isIdentifier() const;
    bool isOperator() const;
    bool isDelimiter() const;

   private:
    TokenType type_;      // token类型
    std::string lexeme_;  // token的词素
    int line_;            // token所在行号
};

// 添加符号表类
class SymbolTable {
   public:
    static bool addSymbol(const std::string& name, const std::string& type);
    static bool lookupSymbol(const std::string& name, std::string& type);
    static void clear();

   private:
    static std::unordered_map<std::string, std::string> symbols_;
};

}  // namespace tiny_compiler
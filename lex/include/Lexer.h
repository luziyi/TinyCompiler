#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Token.h"

namespace tiny_compiler {

class Lexer {
   public:
    explicit Lexer(const std::string& source);

    // 获取下一个token
    Token nextToken();

    // 预览下一个token但不移动指针
    Token peekToken();

    // 获取所有token
    std::vector<Token> getAllTokens();

    // 错误处理
    bool hasError() const { return !errorMessage_.empty(); }
    const std::string& getErrorMessage() const { return errorMessage_; }

   private:
    // 辅助函数
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    void skipWhitespace();
    Token makeToken(TokenType type) const;
    Token errorToken(const std::string& message);

    // 处理各种token的函数
    Token identifier();
    Token number();
    Token string();

    // 判断字符类型的函数
    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isAlphaNumeric(char c);

    // 源代码相关
    std::string source_;
    size_t start_ = 0;
    size_t current_ = 0;
    int line_ = 1;
    std::string errorMessage_;

    // 关键字表
    static const std::unordered_map<std::string, TokenType> keywords_;
};

}  // namespace tiny_compiler
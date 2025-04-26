#include "../include/Lexer.h"

#include <iostream>

namespace tiny_compiler {

// 初始化关键字表
const std::unordered_map<std::string, TokenType> Lexer::keywords_ = {
    {"const", TokenType::CONST},  {"int", TokenType::INT},
    {"float", TokenType::FLOAT},  {"void", TokenType::VOID},
    {"if", TokenType::IF},        {"else", TokenType::ELSE},
    {"return", TokenType::RETURN}};

Lexer::Lexer(const std::string& source)
    : source_(source), current_(0), start_(0), line_(1) {}

Token Lexer::nextToken() {
    skipWhitespace();
    start_ = current_;

    if (isAtEnd()) {
        return makeToken(TokenType::EOF_TOKEN);
    }

    char c = advance();

    if (isAlpha(c) || c == '_') return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        // 单字符token
        case '(':
            return makeToken(TokenType::LPAREN);
        case ')':
            return makeToken(TokenType::RPAREN);
        case '{':
            return makeToken(TokenType::LBRACE);
        case '}':
            return makeToken(TokenType::RBRACE);
        case ';':
            return makeToken(TokenType::SEMICOLON);
        case ',':
            return makeToken(TokenType::COMMA);
        case '+':
            return makeToken(TokenType::PLUS);
        case '-':
            return makeToken(TokenType::MINUS);
        case '*':
            return makeToken(TokenType::MULTIPLY);
        case '%':
            return makeToken(TokenType::MOD);

        // 可能是双字符的token
        case '!':
            return makeToken(match('=') ? TokenType::NEQ : TokenType::NOT);
        case '=':
            return makeToken(match('=') ? TokenType::EQ : TokenType::ASSIGN);
        case '<':
            return makeToken(match('=') ? TokenType::LE : TokenType::LT);
        case '>':
            return makeToken(match('=') ? TokenType::GE : TokenType::GT);
        case '&':
            if (match('&')) return makeToken(TokenType::AND);
            break;
        case '|':
            if (match('|')) return makeToken(TokenType::OR);
            break;
        case '/':
            if (match('/')) {
                // 单行注释
                while (peek() != '\n' && !isAtEnd()) advance();
                return nextToken();
            } else if (match('*')) {
                // 多行注释
                while (!isAtEnd() && !(peek() == '*' && peekNext() == '/')) {
                    if (peek() == '\n') line_++;
                    advance();
                }
                if (!isAtEnd()) {
                    advance();  // 跳过 '*'
                    advance();  // 跳过 '/'
                }
                return nextToken();
            }
            return makeToken(TokenType::DIVIDE);
    }

    return errorToken("Unexpected character.");
}

Token Lexer::identifier() {
    while (isAlphaNumeric(peek())) advance();

    std::string text = source_.substr(start_, current_ - start_);
    auto it = keywords_.find(text);
    TokenType type = it != keywords_.end() ? it->second : TokenType::IDENTIFIER;

    if (type == TokenType::IDENTIFIER) {
        std::string symbolType;
        if (!SymbolTable::lookupSymbol(text, symbolType)) {
            SymbolTable::addSymbol(text, "IDENTIFIER");
        }
    }

    return makeToken(type);
}

Token Lexer::number() {
    bool isFloat = false;

    while (isDigit(peek())) advance();

    // 处理小数点
    if (peek() == '.' && isDigit(peekNext())) {
        isFloat = true;
        advance();  // 跳过小数点

        while (isDigit(peek())) advance();
    }

    std::string numStr = source_.substr(start_, current_ - start_);
    SymbolTable::addSymbol(numStr, isFloat ? "FLOAT_CONST" : "INT_CONST");

    return makeToken(isFloat ? TokenType::FLOAT_CONST : TokenType::INT_CONST);
}

Token Lexer::peekToken() {
    size_t oldCurrent = current_;
    size_t oldStart = start_;
    int oldLine = line_;

    Token token = nextToken();

    current_ = oldCurrent;
    start_ = oldStart;
    line_ = oldLine;

    return token;
}

std::vector<Token> Lexer::getAllTokens() {
    std::vector<Token> tokens;
    while (true) {
        Token token = nextToken();
        tokens.push_back(token);
        if (token.getType() == TokenType::EOF_TOKEN) break;
    }
    return tokens;
}

char Lexer::advance() {
    current_++;
    return source_[current_ - 1];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Lexer::peekNext() const {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_[current_ + 1];
}

bool Lexer::isAtEnd() const { return current_ >= source_.length(); }

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source_[current_] != expected) return false;
    current_++;
    return true;
}

void Lexer::skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line_++;
                advance();
                break;
            default:
                return;
        }
    }
}

Token Lexer::makeToken(TokenType type) const {
    std::string text = source_.substr(start_, current_ - start_);
    return Token(type, text, line_);
}

Token Lexer::errorToken(const std::string& message) {
    errorMessage_ = message + " at line " + std::to_string(line_);
    return Token(TokenType::ERROR, message, line_);
}

bool Lexer::isDigit(char c) { return c >= '0' && c <= '9'; }

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

}  // namespace tiny_compiler
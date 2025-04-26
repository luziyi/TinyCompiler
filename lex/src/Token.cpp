#include "../include/Token.h"

namespace tiny_compiler {

// 初始化静态成员
std::unordered_map<std::string, std::string> SymbolTable::symbols_;

Token::Token(TokenType type, const std::string& lexeme, int line)
    : type_(type), lexeme_(lexeme), line_(line) {}

TokenType Token::getType() const { return type_; }

const std::string& Token::getLexeme() const { return lexeme_; }

int Token::getLine() const { return line_; }

bool Token::isKeyword() const {
    switch (type_) {
        case TokenType::CONST:
        case TokenType::INT:
        case TokenType::FLOAT:
        case TokenType::VOID:
        case TokenType::IF:
        case TokenType::ELSE:
        case TokenType::RETURN:
            return true;
        default:
            return false;
    }
}

bool Token::isIdentifier() const { return type_ == TokenType::IDENTIFIER; }

bool Token::isOperator() const {
    switch (type_) {
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULTIPLY:
        case TokenType::DIVIDE:
        case TokenType::MOD:
        case TokenType::ASSIGN:
        case TokenType::EQ:
        case TokenType::NEQ:
        case TokenType::LT:
        case TokenType::GT:
        case TokenType::LE:
        case TokenType::GE:
        case TokenType::AND:
        case TokenType::OR:
        case TokenType::NOT:
            return true;
        default:
            return false;
    }
}

bool Token::isDelimiter() const {
    switch (type_) {
        case TokenType::LPAREN:
        case TokenType::RPAREN:
        case TokenType::LBRACE:
        case TokenType::RBRACE:
        case TokenType::SEMICOLON:
        case TokenType::COMMA:
            return true;
        default:
            return false;
    }
}

std::string Token::toString() const {
    std::string typeStr;
    switch (type_) {
        case TokenType::CONST:
            typeStr = "CONST";
            break;
        case TokenType::INT:
            typeStr = "INT";
            break;
        case TokenType::FLOAT:
            typeStr = "FLOAT";
            break;
        case TokenType::VOID:
            typeStr = "VOID";
            break;
        case TokenType::IF:
            typeStr = "IF";
            break;
        case TokenType::ELSE:
            typeStr = "ELSE";
            break;
        case TokenType::RETURN:
            typeStr = "RETURN";
            break;
        case TokenType::IDENTIFIER:
            typeStr = "IDENTIFIER";
            break;
        case TokenType::INT_CONST:
            typeStr = "INT_CONST";
            break;
        case TokenType::FLOAT_CONST:
            typeStr = "FLOAT_CONST";
            break;
        case TokenType::PLUS:
            typeStr = "PLUS";
            break;
        case TokenType::MINUS:
            typeStr = "MINUS";
            break;
        case TokenType::MULTIPLY:
            typeStr = "MULTIPLY";
            break;
        case TokenType::DIVIDE:
            typeStr = "DIVIDE";
            break;
        case TokenType::MOD:
            typeStr = "MOD";
            break;
        case TokenType::ASSIGN:
            typeStr = "ASSIGN";
            break;
        case TokenType::EQ:
            typeStr = "EQ";
            break;
        case TokenType::NEQ:
            typeStr = "NEQ";
            break;
        case TokenType::LT:
            typeStr = "LT";
            break;
        case TokenType::GT:
            typeStr = "GT";
            break;
        case TokenType::LE:
            typeStr = "LE";
            break;
        case TokenType::GE:
            typeStr = "GE";
            break;
        case TokenType::AND:
            typeStr = "AND";
            break;
        case TokenType::OR:
            typeStr = "OR";
            break;
        case TokenType::NOT:
            typeStr = "NOT";
            break;
        case TokenType::LPAREN:
            typeStr = "LPAREN";
            break;
        case TokenType::RPAREN:
            typeStr = "RPAREN";
            break;
        case TokenType::LBRACE:
            typeStr = "LBRACE";
            break;
        case TokenType::RBRACE:
            typeStr = "RBRACE";
            break;
        case TokenType::SEMICOLON:
            typeStr = "SEMICOLON";
            break;
        case TokenType::COMMA:
            typeStr = "COMMA";
            break;
        case TokenType::EOF_TOKEN:
            typeStr = "EOF";
            break;
        case TokenType::ERROR:
            typeStr = "ERROR";
            break;
    }
    return "<" + typeStr + "," + lexeme_ + ">";
}

// 符号表方法实现
bool SymbolTable::addSymbol(const std::string& name, const std::string& type) {
    if (symbols_.find(name) != symbols_.end()) {
        return false;  // 符号已存在
    }
    symbols_[name] = type;
    return true;
}

bool SymbolTable::lookupSymbol(const std::string& name, std::string& type) {
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        type = it->second;
        return true;
    }
    return false;
}

void SymbolTable::clear() { symbols_.clear(); }

}  // namespace tiny_compiler
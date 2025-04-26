#include "../include/Token.h"

namespace tiny_compiler {

Token::Token(TokenType type, const std::string& lexeme, int line)
    : type_(type), lexeme_(lexeme), line_(line) {}

TokenType Token::getType() const { return type_; }

const std::string& Token::getLexeme() const { return lexeme_; }

int Token::getLine() const { return line_; }

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
    return "Token{type=" + typeStr + ", lexeme='" + lexeme_ +
           "', line=" + std::to_string(line_) + "}";
}

}  // namespace tiny_compiler
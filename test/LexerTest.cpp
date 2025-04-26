#include <gtest/gtest.h>

#include "../lex/include/Lexer.h"
#include "../lex/include/Token.h"

using namespace tiny_compiler;

// 测试简单的标识符识别
TEST(LexerTest, TestIdentifier) {
    std::string source = "identifier";
    Lexer lexer(source);
    auto tokens = lexer.getAllTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].getType(), TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].getLexeme(), "identifier");
    EXPECT_EQ(tokens[1].getType(), TokenType::EOF_TOKEN);
}

// 测试整数常量识别
TEST(LexerTest, TestIntConstant) {
    std::string source = "123";
    Lexer lexer(source);
    auto tokens = lexer.getAllTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].getType(), TokenType::INT_CONST);
    EXPECT_EQ(tokens[0].getLexeme(), "123");
    EXPECT_EQ(tokens[1].getType(), TokenType::EOF_TOKEN);
}

// 测试关键字识别
TEST(LexerTest, TestKeywords) {
    std::string source = "int float void if else return const";
    Lexer lexer(source);
    auto tokens = lexer.getAllTokens();

    ASSERT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].getType(), TokenType::INT);
    EXPECT_EQ(tokens[1].getType(), TokenType::FLOAT);
    EXPECT_EQ(tokens[2].getType(), TokenType::VOID);
    EXPECT_EQ(tokens[3].getType(), TokenType::IF);
    EXPECT_EQ(tokens[4].getType(), TokenType::ELSE);
    EXPECT_EQ(tokens[5].getType(), TokenType::RETURN);
    EXPECT_EQ(tokens[6].getType(), TokenType::CONST);
    EXPECT_EQ(tokens[7].getType(), TokenType::EOF_TOKEN);
}

// 测试运算符识别
TEST(LexerTest, TestOperators) {
    std::string source = "+ - * / % = == != < > <= >= && || !";
    Lexer lexer(source);
    auto tokens = lexer.getAllTokens();

    ASSERT_EQ(tokens.size(), 16);  // 15个运算符 + 一个EOF
    EXPECT_EQ(tokens[0].getType(), TokenType::PLUS);
    EXPECT_EQ(tokens[1].getType(), TokenType::MINUS);
    EXPECT_EQ(tokens[2].getType(), TokenType::MULTIPLY);
    EXPECT_EQ(tokens[3].getType(), TokenType::DIVIDE);
    EXPECT_EQ(tokens[4].getType(), TokenType::MOD);
    EXPECT_EQ(tokens[5].getType(), TokenType::ASSIGN);
    EXPECT_EQ(tokens[6].getType(), TokenType::EQ);
    EXPECT_EQ(tokens[7].getType(), TokenType::NEQ);
    EXPECT_EQ(tokens[8].getType(), TokenType::LT);
    EXPECT_EQ(tokens[9].getType(), TokenType::GT);
    EXPECT_EQ(tokens[10].getType(), TokenType::LE);
    EXPECT_EQ(tokens[11].getType(), TokenType::GE);
    EXPECT_EQ(tokens[12].getType(), TokenType::AND);
    EXPECT_EQ(tokens[13].getType(), TokenType::OR);
    EXPECT_EQ(tokens[14].getType(), TokenType::NOT);
    EXPECT_EQ(tokens[15].getType(), TokenType::EOF_TOKEN);
}

// 测试分隔符识别
TEST(LexerTest, TestDelimiters) {
    std::string source = "( ) { } ; ,";
    Lexer lexer(source);
    auto tokens = lexer.getAllTokens();

    ASSERT_EQ(tokens.size(), 7);
    EXPECT_EQ(tokens[0].getType(), TokenType::LPAREN);
    EXPECT_EQ(tokens[1].getType(), TokenType::RPAREN);
    EXPECT_EQ(tokens[2].getType(), TokenType::LBRACE);
    EXPECT_EQ(tokens[3].getType(), TokenType::RBRACE);
    EXPECT_EQ(tokens[4].getType(), TokenType::SEMICOLON);
    EXPECT_EQ(tokens[5].getType(), TokenType::COMMA);
    EXPECT_EQ(tokens[6].getType(), TokenType::EOF_TOKEN);
}

// 测试复杂表达式
TEST(LexerTest, TestComplexExpression) {
    std::string source =
        "int max(int a, int b) { if (a > b) return a; else return b; }";
    Lexer lexer(source);
    auto tokens = lexer.getAllTokens();

    ASSERT_EQ(tokens.size(), 25);
    EXPECT_EQ(tokens[0].getType(), TokenType::INT);
    EXPECT_EQ(tokens[1].getType(), TokenType::IDENTIFIER);  // max
    EXPECT_EQ(tokens[2].getType(), TokenType::LPAREN);
    EXPECT_EQ(tokens[3].getType(), TokenType::INT);
    EXPECT_EQ(tokens[4].getType(), TokenType::IDENTIFIER);  // a
    EXPECT_EQ(tokens[5].getType(), TokenType::COMMA);
    EXPECT_EQ(tokens[6].getType(), TokenType::INT);
    EXPECT_EQ(tokens[7].getType(), TokenType::IDENTIFIER);  // b
    EXPECT_EQ(tokens[8].getType(), TokenType::RPAREN);
    EXPECT_EQ(tokens[9].getType(), TokenType::LBRACE);
    EXPECT_EQ(tokens[10].getType(), TokenType::IF);
    EXPECT_EQ(tokens[11].getType(), TokenType::LPAREN);
    EXPECT_EQ(tokens[12].getType(), TokenType::IDENTIFIER);  // a
    EXPECT_EQ(tokens[13].getType(), TokenType::GT);
    EXPECT_EQ(tokens[14].getType(), TokenType::IDENTIFIER);  // b
    EXPECT_EQ(tokens[15].getType(), TokenType::RPAREN);
    EXPECT_EQ(tokens[16].getType(), TokenType::RETURN);
    EXPECT_EQ(tokens[17].getType(), TokenType::IDENTIFIER);  // a
    EXPECT_EQ(tokens[18].getType(), TokenType::SEMICOLON);
    EXPECT_EQ(tokens[19].getType(), TokenType::ELSE);
    EXPECT_EQ(tokens[20].getType(), TokenType::RETURN);
    EXPECT_EQ(tokens[21].getType(), TokenType::IDENTIFIER);  // b
    EXPECT_EQ(tokens[22].getType(), TokenType::SEMICOLON);
    EXPECT_EQ(tokens[23].getType(), TokenType::RBRACE);
    EXPECT_EQ(tokens[24].getType(), TokenType::EOF_TOKEN);
}
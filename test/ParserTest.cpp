#include <gtest/gtest.h>

#include "../syntax/include/LL1Parser.h"

using namespace tiny_compiler;

// 测试简单的变量声明
TEST(ParserTest, TestVariableDeclaration) {
    std::string source = "int a;";
    LL1Parser parser(source);
    bool result = parser.parse();
    EXPECT_TRUE(result);
    if (!result) {
        std::cout << "错误信息: " << parser.getErrorMessage() << std::endl;
    }
}

// 测试简单的函数声明
TEST(ParserTest, TestFunctionDeclaration) {
    std::string source = "int max(int a, int b) { return a; }";
    LL1Parser parser(source);
    bool result = parser.parse();
    EXPECT_TRUE(result);
    if (!result) {
        std::cout << "错误信息: " << parser.getErrorMessage() << std::endl;
    }
}

// 测试if语句
TEST(ParserTest, TestIfStatement) {
    std::string source =
        "int test() { if (a > b) { return a; } else { return b; } }";
    LL1Parser parser(source);
    bool result = parser.parse();
    EXPECT_TRUE(result);
    if (!result) {
        std::cout << "错误信息: " << parser.getErrorMessage() << std::endl;
    }
}

// 测试表达式
TEST(ParserTest, TestExpression) {
    std::string source = "int test() { return a + b * c - d / e; }";
    LL1Parser parser(source);
    bool result = parser.parse();
    EXPECT_TRUE(result);
    if (!result) {
        std::cout << "错误信息: " << parser.getErrorMessage() << std::endl;
    }
}

// 测试错误的语法
TEST(ParserTest, TestInvalidSyntax) {
    std::string source = "int test() { if (a > b) return a }";  // 缺少分号
    LL1Parser parser(source);
    bool result = parser.parse();
    EXPECT_FALSE(result);
    std::cout << "错误信息: " << parser.getErrorMessage() << std::endl;
}

// 测试复杂函数
TEST(ParserTest, TestComplexFunction) {
    std::string source = R"(
        int factorial(int n) {
            if (n <= 1) {
                return 1;
            }
            return n * factorial(n - 1);
        }
    )";
    LL1Parser parser(source);
    bool result = parser.parse();
    EXPECT_TRUE(result);
    if (!result) {
        std::cout << "错误信息: " << parser.getErrorMessage() << std::endl;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
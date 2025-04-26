#include <iostream>
#include <string>

#include "lex/include/Lexer.h"
#include "syntax/include/LL1Parser.h"

using namespace tiny_compiler;

int main() {
    // 使用一个极其简单的测试用例
    std::string source = "int a;";

    // 词法分析
    Lexer lexer(source);
    auto tokens = lexer.getAllTokens();

    std::cout << "词法分析结果：" << std::endl;
    for (const auto& token : tokens) {
        std::cout << token.toString() << std::endl;
    }

    // 语法分析
    LL1Parser parser(source);

    std::cout << "\n语法分析开始..." << std::endl;
    bool success = parser.parse();

    if (success) {
        std::cout << "语法分析成功！" << std::endl;
    } else {
        std::cout << "语法分析失败！" << std::endl;
        std::cout << "错误信息: " << parser.getErrorMessage() << std::endl;
    }

    return 0;
}
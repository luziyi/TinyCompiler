#include <iostream>
#include <string>

#include "Grammar.h"
#include "LL1Converter.h"

void printUsage(const char* programName) {
    std::cout << "用法: " << programName
              << " <输入文法文件> [<输出文法文件>] [<输出First集文件>] "
                 "[<输出Follow集文件>] [<输出预测分析表文件>]"
              << std::endl;
    std::cout << "说明:" << std::endl;
    std::cout << "  <输入文法文件>       - 包含文法产生式的输入文件"
              << std::endl;
    std::cout << "  <输出文法文件>       - 输出转换后的LL1文法到此文件（可选）"
              << std::endl;
    std::cout << "  <输出First集文件>    - 输出First集到此文件（可选）"
              << std::endl;
    std::cout << "  <输出Follow集文件>   - 输出Follow集到此文件（可选）"
              << std::endl;
    std::cout << "  <输出预测分析表文件> - 输出预测分析表到此文件（可选）"
              << std::endl;
    std::cout << "例子:" << std::endl;
    std::cout << "  " << programName
              << " grammar.txt ll1_grammar.txt first.txt follow.txt table.txt"
              << std::endl;
}

int main(int argc, char* argv[]) {
    // 检查命令行参数
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string inputGrammarFile = argv[1];
    std::string outputGrammarFile = (argc > 2) ? argv[2] : "";
    std::string outputFirstFile = (argc > 3) ? argv[3] : "";
    std::string outputFollowFile = (argc > 4) ? argv[4] : "";
    std::string outputTableFile = (argc > 5) ? argv[5] : "";

    // 加载文法
    Grammar grammar;
    std::cout << "从文件 " << inputGrammarFile << " 加载文法..." << std::endl;
    if (!grammar.loadFromFile(inputGrammarFile)) {
        std::cerr << "加载文法失败！" << std::endl;
        return 1;
    }

    std::cout << "原始文法:" << std::endl;
    grammar.print();
    std::cout << std::endl;

    // 转换为LL1文法
    LL1Converter converter(grammar);
    bool success = converter.convertToLL1();

    // 获取转换后的文法
    const Grammar& ll1Grammar = converter.getLL1Grammar();

    // 如果提供了输出文件路径，则保存结果
    if (!outputGrammarFile.empty() && success) {
        std::cout << "保存LL1文法到 " << outputGrammarFile << "..."
                  << std::endl;
        ll1Grammar.saveToFile(outputGrammarFile);
    }

    if (!outputFirstFile.empty() && success) {
        std::cout << "保存First集到 " << outputFirstFile << "..." << std::endl;
        converter.saveFirstSets(outputFirstFile);
    }

    if (!outputFollowFile.empty() && success) {
        std::cout << "保存Follow集到 " << outputFollowFile << "..."
                  << std::endl;
        converter.saveFollowSets(outputFollowFile);
    }

    if (!outputTableFile.empty() && success) {
        std::cout << "保存预测分析表到 " << outputTableFile << "..."
                  << std::endl;
        converter.saveParseTable(outputTableFile);
    }

    return success ? 0 : 1;
}
#include <iostream>
#include <string>
#include <vector>

// 词法分析模块头文件
#include "lexical/FSM.h"
#include "lexical/SymbolTable.h"
#include "lexical/util.h"

// 语法分析模块头文件
#include "syntax/LRItemSet.h"
#include "syntax/LRTable.h"
#include "syntax/SetCalculate.h"
#include "syntax/analysis.h"
#include "syntax/grammar.h"

using namespace std;

// 声明词法分析函数
extern void lexicalAnalysis(string fileName);

// 运行完整的编译流程：词法分析 -> 语法分析
void compileProcess(const string& sourceFileName,
                    const string& grammarFileName) {
    cout << "==================== 编译器开始工作 ====================" << endl;
    cout << "源文件: " << sourceFileName << endl;
    cout << "语法文件: " << grammarFileName << endl;

    // 第一步：词法分析
    cout << "\n[第一步] 开始词法分析..." << endl;
    lexicalAnalysis(sourceFileName);
    cout << "词法分析完成！结果已写入: " << lexicalTxtPath << endl;

    // 第二步：语法分析
    cout << "\n[第二步] 开始语法分析..." << endl;

    // 使用传入的语法文件路径
    vector<string> input = read_from_lexical();

    // 初始化语法分析所需的数据结构
    vector<Production> productions = parseGrammar(grammarFileName);
    nonTerminal nonterminals;
    Terminal terminals;
    getSymbols(productions, nonterminals, terminals);

    // 计算FIRST集和FOLLOW集
    FirstSets FIRST;
    FollowSets FOLLOW;
    calculateFIRST(productions, terminals, FIRST);
    calculateFOLLOW(productions, terminals, FIRST, FOLLOW, nonterminals,
                    terminals);

    // 生成项目集规范族
    vector<vector<Item>> C;
    vector<State> states;
    generate_all_items_set(C, productions, states, nonterminals, terminals);
    states = itemsets(C, productions, states, nonterminals, terminals);

    // 生成语法分析表
    gotoTable gototable;
    actionTable actiontable;
    gototable = fillGotoTable(gototable, states, nonterminals, terminals);
    actiontable = fillActionTable(productions, actiontable, states, FOLLOW,
                                  nonterminals, terminals);

    // 保存表格到文件
    TableUtils::saveGotoTableCSV(gototable, nonterminals,
                                 "../data/goto_table.csv");
    TableUtils::saveActionTableCSV(actiontable, terminals,
                                   "../data/action_table.csv");

    // 打印表格到控制台
    TableUtils::printGotoTable(gototable, nonterminals);
    TableUtils::printActionTable(actiontable, terminals);

    // 执行语法分析
    stack<string> symbol_stack;
    stack<int> states_stack;
    analysis(input, productions, states, states_stack, symbol_stack,
             actiontable, gototable);

    cout << "语法分析完成！" << endl;

    cout << "\n==================== 编译流程结束 ====================" << endl;
}

void printUsage(const char* programName) {
    cout << "用法: " << programName << " [源文件路径] [语法文件路径]" << endl;
    cout << "示例: " << programName
         << " D:/tju-compiler/data/testsample/test.sy "
            "D:/tju-compiler/data/grammar.txt"
         << endl;
    cout << "说明:" << endl;
    cout << "  - 如果不提供参数，将使用默认的源文件和语法文件" << endl;
    cout << "  - 如果只提供一个参数，将其视为源文件路径，使用默认语法文件"
         << endl;
    cout << "  - 如果提供两个参数，第一个为源文件路径，第二个为语法文件路径"
         << endl;
}

int main(int argc, char** argv) {
    string sourceFileName;
    string grammarFileName;

    // 处理命令行参数
    if (argc == 1) {
        cout << "未指定参数，将使用默认文件..." << endl;
        sourceFileName = "../data/testsample/test.sy";
        grammarFileName = "../data/grammar.txt";
    } else if (argc == 2) {
        sourceFileName = argv[1];
        grammarFileName = "../../data/grammar.txt";
        cout << "使用指定的源文件和默认语法文件" << endl;
    } else if (argc == 3) {
        sourceFileName = argv[1];
        grammarFileName = argv[2];
    } else {
        cout << "参数过多!" << endl;
        printUsage(argv[0]);
        return 1;
    }

    // 检查文件是否存在
    FILE* sourceFile = fopen(sourceFileName.c_str(), "r");
    if (!sourceFile) {
        cout << "错误: 无法打开源文件 " << sourceFileName << endl;
        return 1;
    }
    fclose(sourceFile);

    FILE* grammarFile = fopen(grammarFileName.c_str(), "r");
    if (!grammarFile) {
        cout << "错误: 无法打开语法文件 " << grammarFileName << endl;
        return 1;
    }
    fclose(grammarFile);

    // 执行编译流程
    compileProcess(sourceFileName, grammarFileName);

    return 0;
}
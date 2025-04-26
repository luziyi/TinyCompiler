#include <cstdlib>
#include <iostream>
#include <string>

// 引入词法分析器头文件
#include "FSM.h"
#include "SymbolTable.h"
#include "util.h"

// 引入语法分析器头文件
#include "syntax.h"

using namespace std;

void printBanner() {
    cout << "==================================================" << endl;
    cout << "               简易编译器" << endl;
    cout << "==================================================" << endl;
    cout << "       包含功能：词法分析 + 语法分析" << endl;
    cout << "==================================================" << endl;
}

int main(int argc, char** argv) {
    printBanner();

    string fileName;
    if (argc == 1) {
        cout << "未指定源文件，使用默认文件 'test.sy'" << endl;
        fileName = "./data/test.sy";
    } else {
        fileName = argv[1];
        cout << "使用指定源文件：" << fileName << endl;
    }

    cout << "\n第一步：执行词法分析..." << endl;
    // 调用词法分析
    lexicalAnalysis(fileName);

    cout << "词法分析完成，结果保存在 lexical.txt 文件中" << endl;

    cout << "\n第二步：执行语法分析..." << endl;
    // 调用语法分析
    syntax();
    cout << "语法分析完成，结果保存在 syntax_analysis.txt 文件中" << endl;

    cout << "\n编译过程完成！" << endl;

    return 0;
}
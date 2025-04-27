#include "../include/lexical.h"

std::set<std::string> keyword = {"int",   "float", "void", "return",
                                 "const", "main",  "if",   "else"};
std::set<char> boundary = {'(', ')', '{', '}', ';', ','};
std::set<char> operation = {'+', '-', '*', '/', '%', '=', '>', '<'};
std::set<char> operationBeginChar = {'=', '<', '>', '!', '&', '|'};
std::set<std::string> operationOf2Char = {"==", "<=", ">=", "!=", "&&", "||"};
std::map<std::string, int> tokenCodeMap = {
    {"int", TokenCode::KW_INT},     {"void", TokenCode::KW_VOID},
    {"float", TokenCode::KW_FLOAT}, {"return", TokenCode::KW_RETURN},
    {"const", TokenCode::KW_CONST}, {"main", TokenCode::KW_MAIN},
    {"if", TokenCode::KW_IF},       {"else", TokenCode::KW_ELSE},
    {"+", TokenCode::OP_PLUS},      {"-", TokenCode::OP_MINUS},
    {"*", TokenCode::OP_MULTIPLY},  {"/", TokenCode::OP_DIVIDE},
    {"%", TokenCode::OP_PERCENT},   {"=", TokenCode::OP_ASSIGN},
    {">", TokenCode::OP_GT},        {"<", TokenCode::OP_LT},
    {"==", TokenCode::OP_EQUAL},    {"<=", TokenCode::OP_LEQ},
    {">=", TokenCode::OP_GEQ},      {"!=", TokenCode::OP_NEQ},
    {"&&", TokenCode::OP_AND},      {"||", TokenCode::OP_OR},
    {"(", TokenCode::SE_LPARENT},   {")", TokenCode::SE_RPARENT},
    {"{", TokenCode::SE_LBRACES},   {"}", TokenCode::SE_RBARCES},
    {";", TokenCode::SE_SEMICOLON}, {",", TokenCode::SE_COMMA}};

SymbolTable symbolTable = SymbolTable();
FSM NFA = createNFA();
FSM DFA = NFAtoDFA(NFA);
FSM miniDFA = minimizeDFA(DFA);
int lineNum = 0;

// 使用自动机分析token
void analyseToken(std::string token) {
    if (token.length() <= 0) {
        return;
    }
    // 先判断好是不是关键字
    if (isAllLetter(token) && keyword.count(toLower(token))) {
        printToken(token, tokenCodeMap[token], lineNum);
        return;
    }
    /**
     * 思路：
     * 1. 先用正则表达式构造NFA
     * 2. NFA转成DFA
     * 3. DFA最小化
     * 4. 把token扔进最小化的DFA看看返回什么状态--->
     * 标识符!或变量!，以及错误状态，只有这三种可能 注意：
     * 1. 这里只需要返回TokenCode即可
     */

    int tokenCode = identity(miniDFA, token);
    printToken(token, tokenCode, lineNum);
    if (tokenCode == TokenCode::IDN) {
        symbolTable.addSymbol(token);
    }
}

void lexicalAnalysis(std::string fileName) {
    std::cout << "开始词法分析，文件：" << fileName << std::endl;

    FILE* fp;
    fp = fopen(lexicalTxtPath, "w");
    if (fp == NULL) {
        std::cout << "错误：无法打开输出文件 " << lexicalTxtPath << std::endl;
        return;
    }
    fwrite("", 0, 1, fp);
    fclose(fp);

    std::ifstream file;
    file.open(fileName, std::ios::in);
    if (!file.is_open()) {
        std::cout << "错误：无法打开源文件 " << fileName << std::endl;
        return;
    }

    std::cout << "已打开源文件，开始分析..." << std::endl;

    char c;
    std::string token = "";
    /**
     * 算法概括：
     * 除了分割符和已经定义好的界符和运算符之外，其余所有!所有！token序列都应交给自动机判断
     * 所以这里就是要利用分隔符、界符和运算符将程序分开，因为带有运算符的字符串不应该进入自动机
     */
    while ((c = file.get()) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            analyseToken(token);
            token = "";
            if (c == '\n') {
                lineNum++;
            }
            continue;
        } else if (boundary.count(c)) {  // 界符
            analyseToken(token);
            token = std::string(1, c);
            printToken(token, tokenCodeMap[token], lineNum);
            token = "";
            continue;
        } else if (operation.count(c) ||
                   operationBeginChar.count(c)) {  // 运算符
            analyseToken(token);
            if (operationBeginChar.count(c)) {  // 有可能是两个字符组成的运算符
                char nextChar = file.get();
                std::string tryOp = "";
                tryOp += c;
                tryOp += nextChar;
                if (operationOf2Char.count(
                        tryOp)) {  // 真的是两个字符组成的运算符
                    token = tryOp;
                    printToken(token, tokenCodeMap[token], lineNum);
                } else if (operation.count(
                               c)) {  // 普通的由一个字符组成的运算符
                    token = std::string(1, c);
                    printToken(token, tokenCodeMap[token], lineNum);
                    file.putback(nextChar);
                } else {
                    // 出错了，错误token
                    token = std::string(1, c);
                    printToken(token, TokenCode::UNDIFNIE, lineNum);
                    file.putback(nextChar);
                }
            } else {  // 这就是一个字符组成的运算符
                token = std::string(1, c);
                printToken(token, tokenCodeMap[token], lineNum);
            }
            token = "";
        } else {
            token += c;
        }
    }

    std::cout << "文件分析完成，处理最后一个token..." << std::endl;
    analyseToken(token);  // 处理文件结束时可能存在的最后一个token

    std::cout << "准备输出符号表..." << std::endl;
    symbolTable.printTable();

    std::cout << "词法分析完成！" << std::endl;
}

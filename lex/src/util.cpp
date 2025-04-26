#include "../include/util.h"

// 定义词法分析输出文件路径
const char lexicalTxtPath[] = "./result/lexical.txt";

void test() { std::cout << "test" << std::endl; }

std::string toLower(std::string str) {
    std::string result = str;
    for (unsigned int i = 0; i < str.length(); i++) {
        result[i] = tolower(str[i]);
    }
    return result;
}

bool isAllLetter(std::string str) {
    for (unsigned int i = 0; i < str.length(); i++) {
        if (!isalpha(str[i])) {
            return false;
        }
    }
    return true;
}

void printToken(std::string token, int code, int lineNum) {
    FILE *fp = freopen(lexicalTxtPath, "a", stdout);
    std::string result = "";
    result += token;
    result += '\t';
    result += "<";
    switch (code) {
        case TokenCode::KW_INT:
        case TokenCode::KW_FLOAT:
        case TokenCode::KW_VOID:
        case TokenCode::KW_RETURN:
        case TokenCode::KW_CONST:
        case TokenCode::KW_MAIN:
            result += "KW";
            break;
        case TokenCode::OP_PLUS:
        case TokenCode::OP_MINUS:
        case TokenCode::OP_MULTIPLY:
        case TokenCode::OP_DIVIDE:
        case TokenCode::OP_PERCENT:
        case TokenCode::OP_EQUAL:
        case TokenCode::OP_GT:
        case TokenCode::OP_LT:
        case TokenCode::OP_ASSIGN:
        case TokenCode::OP_LEQ:
        case TokenCode::OP_GEQ:
        case TokenCode::OP_NEQ:
        case TokenCode::OP_AND:
        case TokenCode::OP_OR:
            result += "OP";
            break;

        case TokenCode::SE_LPARENT:
        case TokenCode::SE_RPARENT:
        case TokenCode::SE_LBRACES:
        case TokenCode::SE_RBARCES:
        case TokenCode::SE_SEMICOLON:
        case TokenCode::SE_COMMA:
            result += "SE";
            break;

        case TokenCode::IDN:
            result += "Ident";
            break;
        case TokenCode::INT:
            result += "INT";
            break;
        case TokenCode::FLOAT:
            result += "FLOAT";
            break;

        case TokenCode::UNDIFNIE:
            result += "UNDIFNIE";
            break;
    }
    if (code != TokenCode::IDN && code != TokenCode::INT &&
        code != TokenCode::FLOAT) {
        result += "," + std::to_string(code) + ">";
    } else {
        result += "," + token + ">";
    }
    std::cout << result << std::endl;
    fflush(fp);
    fclose(fp);
    freopen("CON", "w", stdout);
    if (code == TokenCode::UNDIFNIE) {
        std::cout << "UNDIFNIED token: " << token << " in line " << lineNum
                  << std::endl;
    }
}

std::set<char> getLetterList(char c) {
    std::set<char> result;
    if (isalpha(c) || c == '_') {
        std::set<char> letterList = {'_'};
        for (char c = 'a'; c <= 'z'; c++) {
            letterList.insert(c);
        }
        for (char c = 'A'; c <= 'Z'; c++) {
            letterList.insert(c);
        }
        return letterList;
    } else if (c == '0') {
        result = {'0'};
        return result;
    } else if (c >= '1' && c <= '9') {
        std::set<char> numList;
        for (char c = '1'; c <= '9'; c++) {
            numList.insert(c);
        }
        return numList;
    } else if (c == '.') {
        // 支持小数点字符
        result = {'.'};
        return result;
    }
    return result;
}
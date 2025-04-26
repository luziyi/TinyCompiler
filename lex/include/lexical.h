#ifndef LEXICAL_H
#define LEXICAL_H

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "FSM.h"
#include "SymbolTable.h"
#include "TokenCodes.h"
#include "util.h"

// 全局变量声明
extern std::set<std::string> keyword;
extern std::set<char> boundary;
extern std::set<char> operation;
extern std::set<char> operationBeginChar;
extern std::set<std::string> operationOf2Char;
extern std::map<std::string, int> tokenCodeMap;
extern SymbolTable symbolTable;
extern FSM NFA;
extern FSM DFA;
extern FSM miniDFA;
extern int lineNum;

// 使用自动机分析token
void analyseToken(std::string token);

// 词法分析入口函数
void lexicalAnalysis(std::string fileName);

#endif  // LEXICAL_H

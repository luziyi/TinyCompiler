#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <set>
#include <string>

#include "TokenCodes.h"
using namespace std;

const char lexicalTxtPath[] = "./result/lexical.txt";

void test();

string toLower(string str);

bool isAllLetter(string str);

void printToken(string token, int code, int lineNum);

set<char> getLetterList(char ch);

// 主词法分析函数
void lexicalAnalysis(std::string fileName);

#endif
#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <set>
#include <string>

#include "TokenCodes.h"
using namespace std;

const char lexicalTxtPath[] = "../data/lexical.txt";

void test();

string toLower(string str);

bool IS_All_Letter(string str);

void printToken(string token, int code, int lineNum);

set<char> getLetterList(char ch);

// 词法分析主函数，供外部调用
void lexicalAnalysis(string fileName);

#endif
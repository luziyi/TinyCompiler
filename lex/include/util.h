#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <set>
#include <string>

#include "TokenCodes.h"

// 词法分析输出文件路径
extern const char lexicalTxtPath[];

// 测试函数
void test();

// 将字符串转换为小写
std::string toLower(std::string str);

// 检查字符串是否全部由字母组成
bool isAllLetter(std::string str);

// 输出词法单元到文件
void printToken(std::string token, int code, int lineNum);

// 获取字符对应的字符集
std::set<char> getLetterList(char ch);

// 主词法分析函数
void lexicalAnalysis(std::string fileName);

#endif
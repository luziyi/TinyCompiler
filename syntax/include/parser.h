#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "grammar.h"

// 词法分析文件路径
extern const char lexFile_path[];
// 语法分析结果文件路径
extern const char syntax_analysis_path[];

// 分析计数器
extern int cnt_analysis;
// 分析栈
extern std::string stack[N];
extern int top;
// 输入字符流
extern std::vector<std::string> chars;

// 获取词法单元
void getChars(std::string str);

// 执行语法分析过程
bool doAnalysis();

// 准备并开始分析
bool analysis();

#endif  // PARSER_H
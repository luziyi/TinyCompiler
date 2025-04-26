#ifndef PARSER_TABLE_H
#define PARSER_TABLE_H

#include <map>
#include <string>
#include <utility>

#include "grammar.h"  // 添加对grammar.h的引用，因为N定义在这里

extern std::pair<int, int> table[N][N];
extern std::map<std::string, int> vn;
extern std::map<std::string, int> vt;
extern int cnt_vn;
extern int cnt_vt;
extern const std::pair<int, int> _00;

// 表输出文件路径
extern const char table_file[];

// 构建预测分析表
bool getTable();

// 输出预测分析表到文件
void print_table();

#endif  // PARSER_TABLE_H
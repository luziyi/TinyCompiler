#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <map>
#include <set>
#include <string>
#include <vector>

// 非终结符和终结符的最大数量
const int N = 100;

// 语法文件路径
extern const char grammer_path[];

// 非终结符和终结符集合
extern std::set<std::string> Vn, Vt;

// 语法映射
extern std::map<std::string, int> Map;
extern std::map<int, std::string> Map2;

// 语法规则向量
extern std::vector<std::vector<std::string>> v[N];
extern int v_cnt;

// 读取语法文件，构建Vn和Vt集合
void getVnVt();

#endif  // GRAMMAR_H
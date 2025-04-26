#ifndef FIRST_FOLLOW_H
#define FIRST_FOLLOW_H

#include <map>
#include <set>
#include <string>

// first集和follow集
extern std::map<std::string, std::set<std::string>> first;
extern std::map<std::string, std::set<std::string>> follow;

// 输出文件路径
extern const char first_file[];
extern const char follow_file[];

// 计算first集
void getFirst(std::string token);

// 计算follow集，返回是否有更新
bool getFollow();

// 输出first集和follow集到文件
void print_first();
void print_follow();

#endif  // FIRST_FOLLOW_H
#ifndef DEFINE_H
#define DEFINE_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// 文法产生式
struct Production {
    string left;  // 产生式左部
    vector<string>
        right;  // 产生式右部---（在grammer.txt中多个选项则用空格分隔）
    bool operator==(const Production &r) const {
        bool ri = true;
        for (int i = 0; i < right.size(); i++)
            if (right[i] != r.right[i]) ri = false;
        return ((left == r.left) && ri);
    }
};

// 项目定义
struct Item {
    int productionIndex;  // 产生式的索引
    int dotPos;           // 项目点的位置---统一从下标0开始
    bool operator==(const Item &other) const {
        return productionIndex == other.productionIndex &&
               dotPos == other.dotPos;
    }
    bool operator<(const Item &other) const {
        if (productionIndex == other.productionIndex)
            return dotPos < other.dotPos;
        else
            return productionIndex < other.productionIndex;
    }
};

// 状态定义
struct State {
    int stateNumber;               // 状态序号
    vector<Item> items;            // 状态中包含的所有项目
    map<string, int> transitions;  // 状态到其他状态的转移关系-相当于GO函数表
};

typedef unordered_set<string> nonTerminal;                         // 非终结符
typedef unordered_set<string> Terminal;                            // 终结符
typedef unordered_map<int, unordered_map<string, int>> gotoTable;  // goto表
typedef unordered_map<int, unordered_map<string, string>>
    actionTable;                                                  // action表
typedef unordered_map<string, unordered_set<string>> FollowSets;  // follow集
typedef unordered_map<string, unordered_set<string>> FirstSets;   // first集

#endif  // DEFINE_H
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
    bool operator==(const Production& r) const {
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
    bool operator==(const Item& other) const {
        return productionIndex == other.productionIndex &&
               dotPos == other.dotPos;
    }
    bool operator<(const Item& other) const {
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
typedef unordered_map<string, unordered_set<string>> FirstSets;   //

// 添加表格保存和打印的辅助函数
class TableUtils {
   public:
    // 保存 GOTO 表到文件
    static bool saveGotoTable(const gotoTable& table,
                              const nonTerminal& nonterminals,
                              const string& filepath) {
        ofstream file(filepath);
        if (!file.is_open()) return false;

        // 写入表头（非终结符）
        for (const auto& nonterm : nonterminals) {
            file << "\t" << nonterm;
        }
        file << endl;

        // 写入每个状态的转移
        for (const auto& state : table) {
            file << state.first;  // 状态编号
            for (const auto& nonterm : nonterminals) {
                file << "\t";
                auto it = state.second.find(nonterm);
                if (it != state.second.end()) {
                    file << it->second;
                } else {
                    file << "-";
                }
            }
            file << endl;
        }
        file.close();
        return true;
    }

    // 保存 ACTION 表到文件
    static bool saveActionTable(const actionTable& table,
                                const Terminal& terminals,
                                const string& filepath) {
        ofstream file(filepath);
        if (!file.is_open()) return false;

        // 写入表头（终结符）
        for (const auto& term : terminals) {
            file << "\t" << term;
        }
        file << endl;

        // 写入每个状态的动作
        for (const auto& state : table) {
            file << state.first;  // 状态编号
            for (const auto& term : terminals) {
                file << "\t";
                auto it = state.second.find(term);
                if (it != state.second.end()) {
                    file << it->second;
                } else {
                    file << "-";
                }
            }
            file << endl;
        }
        file.close();
        return true;
    }

    // 打印 GOTO 表到控制台
    static void printGotoTable(const gotoTable& table,
                               const nonTerminal& nonterminals) {
        cout << "\nGOTO Table:\n";
        // 打印表头
        cout << "State";
        for (const auto& nonterm : nonterminals) {
            cout << "\t" << nonterm;
        }
        cout << endl;

        // 打印每个状态的转移
        for (const auto& state : table) {
            cout << state.first;
            for (const auto& nonterm : nonterminals) {
                cout << "\t";
                auto it = state.second.find(nonterm);
                if (it != state.second.end()) {
                    cout << it->second;
                } else {
                    cout << "-";
                }
            }
            cout << endl;
        }
    }

    // 打印 ACTION 表到控制台
    static void printActionTable(const actionTable& table,
                                 const Terminal& terminals) {
        cout << "\nACTION Table:\n";
        // 打印表头
        cout << "State";
        for (const auto& term : terminals) {
            cout << "\t" << term;
        }
        cout << endl;

        // 打印每个状态的动作
        for (const auto& state : table) {
            cout << state.first;
            for (const auto& term : terminals) {
                cout << "\t";
                auto it = state.second.find(term);
                if (it != state.second.end()) {
                    cout << it->second;
                } else {
                    cout << "-";
                }
            }
            cout << endl;
        }
    }

    static bool saveGotoTableCSV(const gotoTable& table,
                                 const nonTerminal& nonterminals,
                                 const string& filepath) {
        ofstream file(filepath);
        if (!file.is_open()) return false;

        // 写入表头（非终结符）
        file << "State";
        for (const auto& nonterm : nonterminals) {
            file << "," << nonterm;
        }
        file << endl;

        // 写入每个状态的转移
        for (const auto& state : table) {
            file << state.first;  // 状态编号
            for (const auto& nonterm : nonterminals) {
                file << ",";
                auto it = state.second.find(nonterm);
                if (it != state.second.end()) {
                    file << it->second;
                } else {
                    file << "";  // CSV 中空值直接留空
                }
            }
            file << endl;
        }
        file.close();
        return true;
    }

    // 保存 ACTION 表到 CSV 文件
    static bool saveActionTableCSV(const actionTable& table,
                                   const Terminal& terminals,
                                   const string& filepath) {
        ofstream file(filepath);
        if (!file.is_open()) return false;

        // 写入表头（终结符）
        file << "State";
        for (const auto& term : terminals) {
            file << "," << term;
        }
        file << endl;

        // 写入每个状态的动作
        for (const auto& state : table) {
            file << state.first;  // 状态编号
            for (const auto& term : terminals) {
                file << ",";
                auto it = state.second.find(term);
                if (it != state.second.end()) {
                    file << "\"" << it->second
                         << "\"";  // 使用引号包围动作，避免逗号导致的解析问题
                } else {
                    file << "";  // CSV 中空值直接留空
                }
            }
            file << endl;
        }
        file.close();
        return true;
    }
};

#endif  // DEFINE_H
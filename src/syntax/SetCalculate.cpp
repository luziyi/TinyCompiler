#include "syntax/SetCalculate.h"

void calculateFIRST(vector<Production> productions,
                    unordered_set<string> &terminal_symbols,
                    unordered_map<string, unordered_set<string>> &FIRST) {
    terminal_symbols.insert("$");
    for (const auto &production : productions) {
        FIRST[production.left] = unordered_set<string>();  // 初始化FIRST集为空
    }
    bool flag = true;
    while (flag) {
        flag = false;
        for (const auto &production : productions) {
            string left = production.left;
            vector<string> rights = production.right;
            int max_non = 0;
            if (rights.empty()) {
                if (FIRST[left].find("$") == FIRST[left].end()) {
                    FIRST[left].insert("$");
                    flag = true;
                }
                continue;
            }
            for (int index = 0; index < rights.size(); ++index) {
                string current_symbol = rights[index];
                if (terminal_symbols.find(current_symbol) !=
                    terminal_symbols.end()) {
                    if (FIRST[left].find(current_symbol) == FIRST[left].end()) {
                        FIRST[left].insert(current_symbol);
                        flag = true;
                    }
                    max_non = index;
                    break;
                }
                if (FIRST[rights[index]].find("$") ==
                    FIRST[rights[index]].end()) {
                    max_non = index + 1;
                    break;
                }
                if (index == rights.size() - 1) {
                    if (FIRST[left].find("$") == FIRST[left].end()) {
                        FIRST[left].insert("$");
                        flag = true;
                    }
                    max_non = rights.size();
                }
            }
            for (int index = 0; index < max_non; ++index) {
                for (auto &character : FIRST[rights[index]]) {
                    if (character != "$" &&
                        FIRST[left].find(character) == FIRST[left].end()) {
                        FIRST[left].insert(character);
                        flag = true;
                    }
                }
            }
        }
    }
}

void calculateFOLLOW(const vector<Production> productions,
                     unordered_set<string> &terminal_symbols,
                     unordered_map<string, unordered_set<string>> &FIRST,
                     unordered_map<string, unordered_set<string>> &FOLLOW,
                     nonTerminal nonterminals, Terminal terminals) {
    bool has_changes = true;

    // 初始化FOLLOW集
    for (const auto production : productions) {
        FOLLOW[production.left] = unordered_set<string>();
    }

    // 将结束符"#"加入起始符号的FOLLOW集中
    FOLLOW[productions[0].left].insert("#");

    while (has_changes) {
        has_changes = false;
        for (const Production production : productions) {
            string left = production.left;
            vector<string> right = production.right;
            for (int i = 0; i < right.size(); i++) {
                string symbol = right[i];
                if (terminals.find(symbol) != terminals.end()) continue;
                // 处理非终结符symbol的FOLLOW集
                if (i != right.size() - 1)  // 防止越界
                {
                    string next_symbol = right[i + 1];
                    if (nonterminals.find(next_symbol) != nonterminals.end()) {
                        for (const string x : FIRST[next_symbol]) {
                            if (x != "$" && !FOLLOW[symbol].count(x)) {
                                FOLLOW[symbol].insert(x);
                                has_changes = true;
                            }
                        }
                    } else {
                        if (!FOLLOW[symbol].count(next_symbol)) {
                            FOLLOW[symbol].insert(next_symbol);
                            has_changes = true;
                        }
                    }
                }

                // 如果β可以推导出空串或者β是产生式右部的最后一个符号，则将FOLLOW集(left)中的所有元素加入FOLLOW集(symbol)中
                if (i == right.size() - 1 ||
                    (i == right.size() - 2 && FIRST[right[i + 1]].count("$"))) {
                    for (const string t : FOLLOW[left]) {
                        if (!FOLLOW[symbol].count(t)) {
                            FOLLOW[symbol].insert(t);
                            has_changes = true;
                        }
                    }
                }
            }
        }
    }
}
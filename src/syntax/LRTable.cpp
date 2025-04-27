#include "syntax/LRItemSet.h"

gotoTable fillGotoTable(gotoTable &table, const vector<State> &states,
                        nonTerminal nonterminals, Terminal terminals) {
    unordered_set<string> allsymbol;
    allsymbol.insert(nonterminals.begin(), nonterminals.end());
    allsymbol.insert(terminals.begin(), terminals.end());
    // 先把所有位置填上"-1"
    for (const State &state : states) {
        int state_number = state.stateNumber;
        for (const string &symbol : allsymbol) {
            table[state_number][symbol] = -1;
        }
    }
    for (const State &state : states) {
        int state_number = state.stateNumber;
        for (const auto goto_item : state.transitions) {
            // if(A is Nonterminal): k+A-->j，则加入{k,("A",j)}
            string symbol = goto_item.first;
            if (nonterminals.find(symbol) != nonterminals.end())
                table[state_number][symbol] = goto_item.second;
        }
    }
    return table;
}

actionTable fillActionTable(vector<Production> productions, actionTable &table,
                            vector<State> states,
                            unordered_map<string, unordered_set<string>> FOLLOW,
                            nonTerminal nonterminals, Terminal terminals) {
    unordered_set<string> allsymbol;
    allsymbol.insert(nonterminals.begin(), nonterminals.end());
    allsymbol.insert(terminals.begin(), terminals.end());
    // 先把所有位置填上"error"
    for (const State &state : states) {
        int state_number = state.stateNumber;
        for (const string &symbol : allsymbol) {
            table[state_number][symbol] = "error";
        }
    }
    for (const State &state : states) {
        int state_number = state.stateNumber;
        for (const Item &item : state.items) {
            int index = item.productionIndex;
            string left = productions[index].left;
            vector<string> right = productions[index].right;
            unordered_set<string> followset;
            followset = FOLLOW[left];

            // 移进项目
            if (item.dotPos < right.size()) {
                string symbol;
                symbol = right[item.dotPos];

                if (states[state_number].transitions[symbol] != -1)  // 终结符
                {
                    if (terminals.find(symbol) != terminals.end()) {
                        int j = state.transitions.at(symbol);
                        table[state_number][symbol] = "s" + to_string(j);
                    }
                }
            }
            // 规约项目
            else if (item.dotPos == right.size() && item.productionIndex != 0) {
                for (const string symbol : terminals) {
                    if (followset.find(symbol) != followset.end()) {
                        table[state_number][symbol] = "r" + to_string(index);
                    }
                }
            } else if (item.productionIndex == 0 &&
                       item.dotPos == right.size()) {
                table[state_number]["#"] = "acc";
            }
        }
    }
    return table;
}
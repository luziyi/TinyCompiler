#include "syntax/LRItemSet.h"

vector<Item> generateAllItems(vector<Production> productions) {
    vector<Item> items;

    for (int i = 0; i < productions.size(); i++) {
        Production production = productions[i];
        for (int j = 0; j < production.right.size(); j++) {
            Item item;
            item.productionIndex = i, item.dotPos = j;
            items.push_back(item);
        }
        if (!production.right.size()) {
            Item item;
            item.productionIndex = i, item.dotPos = 0;
            items.push_back(item);
        }
    }

    return items;
}

bool GO(vector<Item> &Gotoitems, const vector<Item> Fromstate,
        const string symbol, vector<Production> productions) {
    bool ischanged = false;

    for (const auto item : Fromstate) {
        if (item.dotPos < productions[item.productionIndex].right.size() &&
            productions[item.productionIndex].right[item.dotPos] == symbol) {
            Item newItem;
            newItem.productionIndex = item.productionIndex,
            newItem.dotPos = item.dotPos + 1;
            Gotoitems.push_back(newItem);
            ischanged = true;
        }
    }
    return ischanged;
}

vector<Item> CLOSURE(vector<Item> items, vector<Production> productions,
                     nonTerminal nonterminals) {
    vector<Item> closure = items;  // 初态I0
    vector<Item> closure_copy = closure;
    vector<Item> allitems = generateAllItems(productions);

    bool changed = 0;
    do {
        if (changed) closure = closure_copy;
        changed = false;
        for (const auto item : closure) {
            if (item.dotPos < productions[item.productionIndex].right.size()) {
                string nextSymbol =
                    productions[item.productionIndex].right[item.dotPos];
                if (nonterminals.find(nextSymbol) != nonterminals.end()) {
                    for (const auto newItem : allitems) {
                        if (newItem.dotPos == 0 &&
                            productions[newItem.productionIndex].left ==
                                nextSymbol &&
                            find(closure_copy.begin(), closure_copy.end(),
                                 newItem) == closure_copy.end()) {
                            closure_copy.push_back(newItem);
                            changed = true;
                        }
                    }
                }
            }
        }
    } while (changed);
    return closure;
}

void generate_all_items_set(vector<vector<Item>> &C,
                            vector<Production> productions,
                            vector<State> states, nonTerminal nonterminals,
                            Terminal terminals) {
    vector<vector<Item>> copy;

    vector<Item> S0;
    Item a;
    a.dotPos = 0, a.productionIndex = 0;  // 在第一个项目集里放第一个项目
    S0.push_back(a);
    vector<Item> items = (CLOSURE(
        S0, productions, nonterminals));  // 初始项目集为closure({S'→>'S})
    C.push_back(items);
    unordered_set<string> allsymbol;
    allsymbol.insert(nonterminals.begin(), nonterminals.end());
    allsymbol.insert(terminals.begin(), terminals.end());

    int loopnum = 0;
    copy = C;
    while (true) {
        loopnum++;
        C = copy;
        bool renew = false;
        for (const auto I : C) {
            for (const auto X : allsymbol) {
                vector<Item> goItems;
                bool changed = GO(goItems, I, X, productions);
                if (changed) {
                    if (!goItems.empty()) {
                        goItems = CLOSURE(goItems, productions, nonterminals);
                        if (find(copy.begin(), copy.end(), goItems) ==
                            copy.end()) {
                            // 确保项目集唯一
                            copy.push_back(goItems);
                            renew = true;
                        }
                    }
                }
            }
        }
        if (!renew) {
            break;
        }
    }
}

vector<State> itemsets(vector<vector<Item>> C, vector<Production> productions,
                       vector<State> &states, nonTerminal nonterminals,
                       Terminal terminals) {
    for (int i = 0; i < C.size(); i++) {
        State state;
        state.stateNumber = i;
        state.items = C[i];
        states.push_back(state);
    }
    unordered_set<string> allsymbol;
    allsymbol.insert(nonterminals.begin(), nonterminals.end());
    allsymbol.insert(terminals.begin(), terminals.end());

    // 转移关系初始化为全-1
    for (int i = 0; i < states.size(); i++) {
        for (const auto X : allsymbol) {
            states[i].transitions[X] = -1;
        }
    }
    // 构建转移关系
    for (int i = 0; i < states.size(); i++) {
        const auto &state = states[i];
        for (const auto X : allsymbol) {
            vector<Item> goItems;
            bool ischanged = GO(goItems, state.items, X, productions);
            if (ischanged) {
                for (int j = 0; j < states.size(); j++) {
                    for (const Item item : states[j].items)
                        if (item == goItems[0]) {
                            states[i].transitions[X] = j;
                            break;
                        }
                }
            }
        }
    }
    return states;
}
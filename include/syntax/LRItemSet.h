#ifndef ITEMSET_H
#define ITEMSET_H

#include "syntax/grammar.h"

vector<Item> generateAllItems(vector<Production> productions);

bool GO(vector<Item> &Gotoitems, const vector<Item> Fromstate,
        const string symbol, vector<Production> productions);

vector<Item> CLOSURE(vector<Item> items, vector<Production> productions,
                     nonTerminal nonterminals);

void generate_all_items_set(vector<vector<Item>> &C,
                            vector<Production> productions,
                            vector<State> states, nonTerminal nonterminals,
                            Terminal terminals);

vector<State> itemsets(vector<vector<Item>> C, vector<Production> productions,
                       vector<State> &states, nonTerminal nonterminals,
                       Terminal terminals);
#endif

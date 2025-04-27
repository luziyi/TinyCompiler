#ifndef LRTABLE_H
#define LRTABLE_H

#include "syntax/grammar.h"

gotoTable fillGotoTable(gotoTable &table, const vector<State> &states,
                        nonTerminal nonterminals, Terminal terminals);

actionTable fillActionTable(vector<Production> productions, actionTable &table,
                            vector<State> states,
                            unordered_map<string, unordered_set<string>> FOLLOW,
                            nonTerminal nonterminals, Terminal terminals);

#endif
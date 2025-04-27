#ifndef SETCALC_H
#define SETCALC_H
#include "syntax/grammar.h"

void calculateFIRST(vector<Production> productions,
                    unordered_set<string> &terminal_symbols,
                    unordered_map<string, unordered_set<string>> &FIRST);

void calculateFOLLOW(const vector<Production> productions,
                     unordered_set<string> &terminal_symbols,
                     unordered_map<string, unordered_set<string>> &FIRST,
                     unordered_map<string, unordered_set<string>> &FOLLOW,
                     nonTerminal nonterminals, Terminal terminals);

#endif

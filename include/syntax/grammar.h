#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "syntax/define.h"

using namespace std;

Production splitProduction(const string &input, const string &delimiter);

vector<Production> parseGrammar(const string &filename);

void getSymbols(const vector<Production> &productions,
                unordered_set<string> &nonTerminal,
                unordered_set<string> &Terminal);

#endif  // GRAMMAR_H
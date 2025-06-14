#ifndef SYNTAX_H
#define SYNTAX_H

#include "syntax/grammar.h"

vector<string> read_from_lexical();

void analysis(vector<string> rest_string, vector<Production> &productions,
                 vector<State> states, stack<int> &states_stack,
                 stack<string> &symbol_stack, actionTable actiontable,
                 gotoTable gototable);

#endif
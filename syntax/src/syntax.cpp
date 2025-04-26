#include "../include/syntax.h"

#include <iostream>

#include "../include/first_follow.h"
#include "../include/grammar.h"
#include "../include/parser.h"
#include "../include/parser_table.h"

void syntax() {
    getVnVt();
    getFirst("program");
    print_first();
    while (getFollow());
    print_follow();
    if (getTable()) {
        puts("the grammer is not LL(1)");
        exit(0);
    }
    print_table();
    if (!analysis()) {
        puts("the lexical info has error(s)");
    }
}

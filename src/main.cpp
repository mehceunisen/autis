#include <iostream>

#include "lexer.h"
#include "parser.h"

int main() {
    Lexer l("file.ats");
    Parser p(l);
    auto res = static_cast<ExpressionAST*>(p.parse()); 
    return 0;
}


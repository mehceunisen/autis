#include <iostream>

#include "lexer.h"
#include "parser.h"

int main() {
    Lexer l("file.ats");
    Parser p(l);
    for (int i = 0; i < 10; ++i) {
        auto res = static_cast<ExpressionAST*>(p.parse());
    }
    return 0;
}


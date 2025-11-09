#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "expression_ast.h"
#include "statement_ast.h"
int main() {
    Lexer l("file.ats");
    Parser p(l);
    for (int i = 0; i < 10; ++i) {
        auto res = static_cast<AssignmentASTNode*>(p.parse());
    }
    return 0;
}


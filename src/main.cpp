#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "ast_util.h"
#include "expression_ast.h"
#include "statement_ast.h"
int main() {
    Lexer l("file.ats");
    Parser p(l);
    for (int i = 0; i < 10; ++i) {
        auto res = unique_ptr_cast<AssignmentASTNode>(p.parse());
        std::cout << "of\n";
    }
    return 0;
}


#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "ast_util.h"
#include "expression_ast.h"
#include "statement_ast.h"
#include "token.h"

int main() {
    Lexer l("file.ats");
    Parser p(l);
    std::vector<std::unique_ptr<StatementAST>> res;
    while(auto r = p.parse()) {
      res.emplace_back(std::move(r));
    }
    return 0;
}


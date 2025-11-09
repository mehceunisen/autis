#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "lexer.h"
#include "ast_node.h"


class Parser {
public:
    Parser(Lexer& lexer);

    ASTNode* parse();

private:
    ExpressionAST* parse_primary();

    ExpressionAST* parse_literal(); 
    ExpressionAST* parse_type();
    ExpressionAST* parse_binary_op();

    StatementAST* parse_control();
    StatementAST* parse_func();

    Lexeme advance_lexeme();

    Lexeme current_lxm_;
    Lexeme next_lxm_;

    Lexer& lexer_;
};

#endif

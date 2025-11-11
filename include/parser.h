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
    ASTNode* parse_primary();

    ExpressionAST* parse_literal(); 
    ExpressionAST* parse_type();
    ExpressionAST* parse_binary_op();
    ExpressionAST* parse_binary_op_rhs(int exper_prec, ExpressionAST* lhs);

    StatementAST* parse_control();
    StatementAST* parse_function_def();

    Lexeme advance_lexeme();

    Lexeme current_lxm_;
    Lexeme next_lxm_;

    Lexer& lexer_;
};

#endif

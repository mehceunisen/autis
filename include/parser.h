#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "lexer.h"
#include "ast_node.h"


class Parser {
public:
    Parser(Lexer& lexer);

    std::unique_ptr<StatementAST> parse();
private:
    std::unique_ptr<ExpressionAST> parse_primary();

    std::unique_ptr<ExpressionAST> parse_literal(); 
    std::unique_ptr<ExpressionAST> parse_type();
    std::unique_ptr<ExpressionAST> parse_binary_op();
    std::unique_ptr<ExpressionAST> parse_function_call();
    std::unique_ptr<ExpressionAST> parse_unary_expression();
    std::unique_ptr<ExpressionAST> parse_binary_op_rhs(int exper_prec, std::unique_ptr<ExpressionAST> lhs);
    std::unique_ptr<ExpressionAST> parse_identifier();

    std::unique_ptr<StatementAST> parse_control();
    std::unique_ptr<StatementAST> parse_function_def();
    std::unique_ptr<StatementAST> parse_variable_declaration();

    Lexeme advance_lexeme();

    Lexeme current_lxm_;
    Lexeme prev_lxm_;

    Lexer& lexer_;
};

#endif

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class StatementAST : public ASTNode {
public:
    StatementAST(int64_t v) : val(v) {}
    int64_t val;
};

class ExpressionAST : public ASTNode {
public:
    ExpressionAST(std::unique_ptr<ASTNode> lhs, std::unique_ptr<ASTNode> rhs)
        : lhs(std::move(lhs)), rhs(std::move(rhs)) {}
    std::unique_ptr<ASTNode> lhs, rhs;
};


class Parser {
public:
    Parser(Lexer& lexer);

    void parse();

private:
    std::unique_ptr<ASTNode> parse_statement();
    std::unique_ptr<ASTNode> parse_expression(std::unique_ptr<ASTNode> lhs);
    Lexeme push_lexeme(Lexeme lxm);

    Lexeme current_lxm_;
    Lexeme next_lxm_;

    Lexer& lexer_;
};

#endif

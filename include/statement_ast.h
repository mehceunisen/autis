#ifndef STATEMENT_AST_H
#define STATEMENT_AST_H

#include "ast_node.h"
#include "token.h"

class VariableDeclarationASTNode : public StatementAST {
public:
    VariableDeclarationASTNode(std::string _name, Token _token, 
            ExpressionAST* _init=nullptr) 
        : name(std::move(_name)), token(_token), init(_init){}
private:
    std::string name;
    Token token;
    ExpressionAST* init;
};

class AssignmentASTNode : public StatementAST {
public:
    AssignmentASTNode(std::string _name, ExpressionAST* _value) 
        : name(std::move(_name)), value(_value) {}
private:
    std::string name;
    ExpressionAST* value;
};

#endif

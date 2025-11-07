#ifndef EXPRESSION_AST_H
#define EXPRESSION_AST_H

#include <vector>

#include "ast_node.h"

class IntASTNode : public ExpressionAST {
public:
    IntASTNode(int64_t v) : val(v) {}
    int64_t get_val() const { return val; };
private:
    int64_t val;
};

class FloatASTNode : public ExpressionAST {
public:
    FloatASTNode(float v) : val(v) {}
    float get_val() const { return val; };
private:
    float val;
};

class StringASTNode : public ExpressionAST {
public:
    StringASTNode(std::string v) : val(std::move(v)) {}
    const std::string& get_val() const { return val; };
private:
    std::string val;
};

class IdentifierASTNode : public ExpressionAST {
public:
    IdentifierASTNode(std::string v) : val(std::move(v)) {}
    const std::string& get_val() const { return val; } 
private:
    std::string val;
};

class OperatorASTNode : public ExpressionAST {
public:
    OperatorASTNode(Token _op_token, ASTNode* _lhs, ASTNode* _rhs) 
        : op_token(_op_token), lhs(_lhs), rhs(_rhs) {}
private: 
    Token op_token;
    ASTNode *lhs, *rhs;
};

class UnaryOpASTNode : public ExpressionAST {
public:
    UnaryOpASTNode(Token _op_token, ExpressionAST* _operand) 
        : op_token(_op_token), operand(_operand) {}
private:
    Token op_token;
    ExpressionAST* operand;
};

class FuncCallASTNode : public ExpressionAST {
public:
    FuncCallASTNode(std::string _name, std::vector<ExpressionAST*> _arguments)      : name(_name), arguments(_arguments) {}
private:
    std::string name;
    std::vector<ExpressionAST*> arguments;
};

#endif

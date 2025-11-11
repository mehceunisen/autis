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

class BinaryOpASTNode : public ExpressionAST {
public:
    BinaryOpASTNode(Token _op_token, std::unique_ptr<ASTNode> _lhs, std::unique_ptr<ASTNode> _rhs) 
        : op_token(_op_token), lhs(std::move(_lhs)), rhs(std::move(_rhs)) {}
private: 
    Token op_token;
    std::unique_ptr<ASTNode> lhs, rhs;
};

class UnaryOpASTNode : public ExpressionAST {
public:
    UnaryOpASTNode(Token _op_token, std::unique_ptr<ExpressionAST> _operand) 
        : op_token(_op_token), operand(std::move(_operand)) {}
private:
    Token op_token;
    std::unique_ptr<ExpressionAST> operand;
};

class FuncCallASTNode : public ExpressionAST {
public:
    FuncCallASTNode(std::string _name, std::vector<ExpressionAST*> _arguments) 
        : name(_name), arguments(_arguments) {}
private:
    std::string name;
    std::vector<ExpressionAST*> arguments;
};

#endif

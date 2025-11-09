#ifndef STATEMENT_AST_H
#define STATEMENT_AST_H

#include <vector>

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

class IfStatementASTNode : public StatementAST {
public:
    IfStatementASTNode(ExpressionAST* _cond, 
                       std::vector<StatementAST*> _then_block, 
                       std::vector<StatementAST*> _else_block) 
        : cond(_cond), then_block(std::move(_then_block)),
        else_block(std::move(_else_block)) {}
private:
    ExpressionAST* cond;
    std::vector<StatementAST*> then_block; 
    std::vector<StatementAST*> else_block;
};

class WhileStatementASTNode : public StatementAST {
public:
    WhileStatementASTNode(ExpressionAST* cond,
                      std::vector<StatementAST*> b)
        : condition(std::move(cond)), body(std::move(b)) {}
private:
    ExpressionAST* condition;
    std::vector<StatementAST*> body;
};

class ForStatementASTNode : public StatementAST {
public:
 ForStatementASTNode(StatementAST* initStmt, ExpressionAST* condExpr,
                    ExpressionAST* incrExpr, 
                    std::vector<StatementAST*> loopBody)
        : init(initStmt),
          condition(condExpr),
          increment(incrExpr),
          body(std::move(loopBody)) {}
private:
    StatementAST* init;      // e.g., int i = 0
    ExpressionAST* condition; // e.g., i < 10
    ExpressionAST* increment; // e.g., i++
    std::vector<StatementAST*> body;
};

class ReturnStatementASTNode : public StatementAST {
public:
    ReturnStatementASTNode(ExpressionAST* val = nullptr)
        : returnValue(std::move(val)) {}
private:
    ExpressionAST* returnValue;  // optional (for void returns)

};

class FunctionDefASTNode : public StatementAST {
private:
    struct Parameter {
        Token type;
        std::string name;
    };
public:
    FunctionDefASTNode(std::string funcName, std::vector<Parameter> params,
                        Token retType, std::vector<StatementAST*> funcBody)
        : name(funcName), parameters(params), returnType(retType),
        body(funcBody) {}
private:

    std::string name;
    std::vector<Parameter> parameters;
    Token returnType;  // or separate enum
    std::vector<StatementAST*> body;
};

#endif

#ifndef STATEMENT_AST_H
#define STATEMENT_AST_H

#include <vector>

#include "ast_node.h"
#include "token.h"

class ExpressionStatementASTNode : public StatementAST {
public:
    ExpressionStatementASTNode(std::unique_ptr<ExpressionAST> _expr) :
        expr(std::move(_expr)) {}
private:
    std::unique_ptr<ExpressionAST> expr;
};

class VariableDeclarationASTNode : public StatementAST {
public:
    VariableDeclarationASTNode(std::string _name, Token _token,
            std::unique_ptr<ExpressionAST> _init=nullptr) 
        : name(std::move(_name)), token(_token), init(std::move(_init)){}
private:
    std::string name;
    Token token;
    std::unique_ptr<ExpressionAST> init;
};

class IfStatementASTNode : public StatementAST {
public:
    IfStatementASTNode(std::unique_ptr<ExpressionAST> _cond, 
                       std::vector<std::unique_ptr<StatementAST>> _then_block, 
                       std::vector<std::unique_ptr<StatementAST>> _else_block) 
        : cond(std::move(_cond)), then_block(std::move(_then_block)),
        else_block(std::move(_else_block)) {}
private:
    std::unique_ptr<ExpressionAST> cond;
    std::vector<std::unique_ptr<StatementAST>> then_block; 
    std::vector<std::unique_ptr<StatementAST>> else_block;
};

class WhileStatementASTNode : public StatementAST {
public:
    WhileStatementASTNode(std::unique_ptr<ExpressionAST> cond,
                      std::vector<std::unique_ptr<StatementAST>> b)
        : condition(std::move(cond)), body(std::move(b)) {}
private:
    std::unique_ptr<ExpressionAST> condition;
    std::vector<std::unique_ptr<StatementAST>> body;
};

class ForStatementASTNode : public StatementAST {
public:
 ForStatementASTNode(std::unique_ptr<StatementAST> initStmt, std::unique_ptr<ExpressionAST> condExpr,
                    std::unique_ptr<ExpressionAST> incrExpr, 
                    std::vector<std::unique_ptr<StatementAST>> loopBody)
        : init(std::move(initStmt)),
          condition(std::move(condExpr)),
          increment(std::move(incrExpr)),
          body(std::move(loopBody)) {}
private:
    std::unique_ptr<StatementAST> init;      // e.g., int i = 0
    std::unique_ptr<ExpressionAST> condition; // e.g., i < 10
    std::unique_ptr<ExpressionAST> increment; // e.g., i++
    std::vector<std::unique_ptr<StatementAST>> body;
};

class ReturnStatementASTNode : public StatementAST {
public:
    ReturnStatementASTNode(std::unique_ptr<StatementAST> val)
        : returnValue(std::move(val)) {}
private:
    std::unique_ptr<StatementAST> returnValue;  // optional (for void returns)

};

class FunctionDefASTNode : public StatementAST {
public:
    struct Parameter {
        Token type;
        std::string name;
    };

    FunctionDefASTNode(std::string funcName, std::vector<Parameter> params,
                        Token retType, std::vector<std::unique_ptr<StatementAST>> funcBody)
        : name(funcName), parameters(params), returnType(retType),
        body(std::move(funcBody)) {}
private:

    std::string name;
    std::vector<Parameter> parameters;
    Token returnType;  // or separate enum
    std::vector<std::unique_ptr<StatementAST>> body;
};

#endif


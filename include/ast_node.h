#ifndef AST_NODE_H
#define AST_NODE_H

#include "token.h"

#include <cstdint>
#include <string>

class SemanticAnalyzer;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(SemanticAnalyzer& analyzer) = 0;
};

class ExpressionAST : public ASTNode {};


class StatementAST : public ASTNode {};

#endif

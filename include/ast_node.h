#ifndef AST_NODE_H
#define AST_NODE_H

#include <cstdint>
#include <string>

#include "token.h"

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class ExpressionAST : public ASTNode {};

class StatementAST : public ASTNode {};

#endif

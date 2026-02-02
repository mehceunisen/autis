#ifndef SYMBOL_H
#define SYMBOL_H

#include "token.h"
#include "ast_node.h"

#include <string>
#include <vector>

class Scope;

class Symbol {
public:
  virtual ~Symbol() = default;
  std::string name;
  Token* token;
};

class VariableDeclarationSymbol : public Symbol { 
public:
  Scope* scope;
};

class FunctionDeclarationSymbol : public Symbol {
public:
  Token* return_type;
  std::vector<ExpressionAST> parameters;
};

#endif

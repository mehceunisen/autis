#ifndef SYMBOL_INFO_H
#define SYMBOL_INFO_H

#include "token.h"
#include "expression_ast.h"

#include <string>

struct Scope;

struct SymbolInfo {
  virtual ~SymbolInfo() = default;
  SymbolInfo(const std::string& name, Token token) : name(name), token(token) { }
  std::string name;
  Token token;
};

class VariableDeclarationSymbolInfo : public SymbolInfo { 
public:
  Scope* scope;
};

class FunctionDeclarationSymbolInfo : public SymbolInfo {
public:
  Token* return_type;
  std::vector<ExpressionAST> parameters;
};

#endif

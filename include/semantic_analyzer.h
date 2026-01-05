#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "token.h"

#include <unordered_map>
#include <memory>
#include <string>

struct SymbolInfo {
  std::string name;
  Token token;
  int offset; // will be used in codegen later
};

struct Scope {
  std::unique_ptr<Scope> parent;
  std::unordered_map<std::string, SymbolInfo> symbols;
};


class SemanticAnalyzer {

};



#endif

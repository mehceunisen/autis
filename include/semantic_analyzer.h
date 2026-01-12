#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "token.h"
#include "ast_node.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <utility>

typedef std::pair<std::string, Token> SymbolInfo;

struct Scope {
  std::shared_ptr<Scope> parent;
  std::unordered_map<std::string, SymbolInfo> symbols;
};

class SemanticAnalyzer {
public:
  SemanticAnalyzer(std::vector<std::unique_ptr<StatementAST>>&& prog);
  std::vector<std::string> analyze();

private:
  // main intention is to lookup parent scopes (if needed)
  bool lookup_symbol(const std::string& symbol_identifier);
  // again, main intention is to lookup (if needed) recursively
  bool lookup_type_compatibility(Token current_type, const SymbolInfo& rhs);
  // AST representation of program
  std::vector<std::unique_ptr<StatementAST>> prog_; 
  // Scope representation of AST currently being worked on
  std::shared_ptr<Scope> global_scope_; 
  // Scope that keeps track of current scope
  std::shared_ptr<Scope> current_scope_;
  // in case of semantic error, save the error string and then print
  std::vector<std::string> errors_;
};

#endif

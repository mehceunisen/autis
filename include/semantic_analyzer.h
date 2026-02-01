#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "token.h"
#include "ast_node.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

struct SymbolInfo {
  std::string name;
  Token token;
};

struct Scope {
  std::shared_ptr<Scope> parent;
  std::unordered_map<std::string, SymbolInfo> symbols;
};

class SemanticAnalyzer {
public:
  // vector of errors containing error string and line
  std::vector<std::pair<std::string, std::size_t>> analyze(std::vector<std::unique_ptr<StatementAST>> program);
  
  // run type check on node current node
  std::pair<std::string, std::size_t> check_type();

  std::pair<std::string, std::size_t> check_variable_declaration();
  std::pair<std::string, std::size_t> check_assignment();
  std::pair<std::string, std::size_t> check_binary_op();
  std::pair<std::string, std::size_t> check_return_type();

  // lookup for the symbol in current and parent scopes
  std::pair<std::string, std::size_t> check_scope();

private:
  // keep track of what scope I'm currently in
  std::shared_ptr<Scope> current_scope_;
};



#endif

#ifndef SEMANTIC_INFO_H
#define SEMANTIC_INFO_H

#include "ast_node.h"
#include "symbol_info.h"

#include <unordered_map>

class SemanticInfo {
public:
  SemanticInfo();
  ~SemanticInfo() = default;

  // don't allow any copy operation
  SemanticInfo(const SemanticInfo& other) = delete;
  SemanticInfo& operator=(const SemanticInfo& other) = delete;
  
  const SymbolInfo* get_symbol(ASTNode* node) {
    if (node_symbol_info.contains(node)) {
      return node_symbol_info[node];
    }
    return nullptr;
  }
  void set_symbol(ASTNode* node, SymbolInfo* symbol) {
    node_symbol_info[node] = symbol;
  }

private:
  std::unordered_map<ASTNode*, SymbolInfo*> node_symbol_info;
};
#endif

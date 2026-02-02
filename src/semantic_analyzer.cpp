#include "semantic_analyzer.h"
#include "symbol.h"
#include "symbol_info.h"
#include "token.h"

#include <memory>
#include <type_traits>
#include <format>

SemanticAnalyzer::SemanticAnalyzer() : 
  main_scope_(std::make_shared<Scope>()),
  current_scope_(main_scope_) {
}

std::vector<std::pair<std::string, std::size_t>> 
  SemanticAnalyzer::analyze(std::vector<std::unique_ptr<StatementAST>> program) {
  // main loop, don't know what to with it lmaooo
}

std::pair<std::string, std::size_t> SemanticAnalyzer::check_scope() {
  // I'm not sure this is necessary 
}

bool SemanticAnalyzer::is_compatible(const Token& t1, const Token& t2) {
  if ((t1 == StringLiteral && t2 != StringLiteral) || 
      (t1 != StringLiteral && t2 == StringLiteral)) { // that's my only case :)
    return false;
  }

  return true;
}

Token SemanticAnalyzer::infer_result_type(const Token operand, const Token& lhs, const Token& rhs) {
  if (operand == OpDiv) {
    return FloatLiteral;
  }

  if ((lhs == IntLiteral || rhs == IntLiteral) && (rhs == FloatLiteral || lhs == FloatLiteral)) {
    return FloatLiteral;
  }
  else if (lhs == IntLiteral && rhs == IntLiteral) {
    return IntLiteral;
  }
  else if (rhs == FloatLiteral && lhs == FloatLiteral) {
    return FloatLiteral;
  }
  else if (rhs == StringLiteral && lhs == StringLiteral) {
    return StringLiteral;
  }

  return FloatLiteral;
}

std::optional<std::string> SemanticAnalyzer::visit(IntASTNode& node) {
  semantic_info_.set_symbol(&node, new SymbolInfo("IntNode", IntLiteral));
  return std::nullopt;
}

std::optional<std::string> SemanticAnalyzer::visit(FloatASTNode& node) {
  semantic_info_.set_symbol(&node, new SymbolInfo("FloatNode", FloatLiteral));
  return std::nullopt;
}

std::optional<std::string> SemanticAnalyzer::visit(StringASTNode& node) {
  semantic_info_.set_symbol(&node, new SymbolInfo("StringNode", StringLiteral));
  return std::nullopt;
}

std::optional<std::string> SemanticAnalyzer::visit(IdentifierASTNode& node) {
  if (!declarations_.contains(&node)) {
    return std::format("No symbol named: {} have found", node.get_val());
  }

  std::shared_ptr<Scope> iter_scope = current_scope_;
  VariableDeclarationSymbol* var_decl_sym = dynamic_cast<VariableDeclarationSymbol*>(declarations_[&node]);

  if(var_decl_sym == nullptr) {
    return std::format("Found symbol with name {} is not a variable", node.get_val());
  } 

  while (iter_scope != nullptr && iter_scope.get() != var_decl_sym->scope) {
    iter_scope = iter_scope->parent;
  }
  
  if (iter_scope == nullptr) {
    return std::format("Symbol {} is unreachable", node.get_val());
  }

  semantic_info_.set_symbol(&node, new SymbolInfo("Identifier", Identifier));
  return std::nullopt;
}

std::optional<std::string> SemanticAnalyzer::visit(UnaryOpASTNode& node) {
  auto id = node.get_identifier();
  if (semantic_info_.get_symbol(&id)->token != IntLiteral) {
    return std::format("No valid Unary operator is defined for {} type", id.get_val());
  }
  
  semantic_info_.set_symbol(&node, new SymbolInfo("UnaryOp", node.get_operator_token()));

  return std::nullopt;
}

std::optional<std::string> SemanticAnalyzer::visit(FuncCallASTNode& node) {
  auto args = node.get_arguments();
  auto func_name = node.get_func_call_name();

  // find func_name, if found, compare args' types with func declaration's parameters
  std::shared_ptr<Scope> iter_scope = current_scope_;
  while (iter_scope != nullptr && 
      iter_scope->symbols.find(func_name)->second.name != "FunctionDeclaration") {
    iter_scope = iter_scope->parent;
  }

  if (iter_scope == nullptr) {
    return std::format("No function declaration found in name {}", node.get_func_call_name());
  }
  
  found_func_decl = iter_scope->symbols.find(func_name);
  
}

std::optional<std::string> SemanticAnalyzer::visit(BinaryOpASTNode& node) {
  node.get_lhs().accept(*this); // call for each nodes visitor recurisvely
  node.get_lhs().accept(*this); // call for each nodes visitor recurisvely

  const SymbolInfo* lhs_info = semantic_info_.get_symbol(&node.get_lhs());
  const SymbolInfo* rhs_info = semantic_info_.get_symbol(&node.get_rhs());
  
  if (lhs_info == nullptr || rhs_info == nullptr) {
    return std::format("Expected binary node, found null");
  }

  if (!is_compatible(lhs_info->token, rhs_info->token)) {
    return std::format("Binary operation types are not compatible");
  }

  Token result_type = infer_result_type(node.get_operand(), lhs_info->token, rhs_info->token);
  semantic_info_.set_symbol(&node, new SymbolInfo("BinOP", result_type));
}

std::optional<std::string> SemanticAnalyzer::visit(ExpressionStatementASTNode& node) {

}

std::optional<std::string> SemanticAnalyzer::visit(VariableDeclarationASTNode& node) {

}

std::optional<std::string> SemanticAnalyzer::visit(AssignmentASTNode& node) {

}

std::optional<std::string> SemanticAnalyzer::visit(IfStatementASTNode& node) {

}

std::optional<std::string> SemanticAnalyzer::visit(WhileStatementASTNode& node) {

}

std::optional<std::string> SemanticAnalyzer::visit(ForStatementASTNode& node) {

}

std::optional<std::string> SemanticAnalyzer::visit(ReturnStatementASTNode& node) {

}

std::optional<std::string> SemanticAnalyzer::visit(FunctionDefASTNode& node) {

}

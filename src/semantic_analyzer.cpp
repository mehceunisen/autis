#include "semantic_analyzer.h"

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

std::optional<std::string> SemanticAnalyzer::visit(IntASTNode& node) {
  auto val = node.get_val();
  if (std::is_same_v<int64_t, decltype(val)>) {
    return std::nullopt;
  }
  return "IntASTNode doesn't contain int64_t";
}

std::optional<std::string> SemanticAnalyzer::visit(FloatASTNode& node) {
  auto val = node.get_val();
  if (std::is_same_v<float, decltype(val)>) {
    return std::nullopt;
  }
  return "FloatASTNode doesn't contain int64_t";
}

std::optional<std::string> SemanticAnalyzer::visit(StringASTNode& node) {
  if (std::is_same_v<std::string, decltype(node.get_val())>) {
    return std::nullopt;
  }
  return "StringASTNode doesn't contain string";
}

std::optional<std::string> SemanticAnalyzer::visit(IdentifierASTNode& node) {
  auto val = node.get_val();
  auto iter_scope = current_scope_;
  while(iter_scope != nullptr && !iter_scope->symbols.contains(val)) {
    iter_scope = iter_scope->parent;
  }

  if (!iter_scope) {
    return std::format("No symbol named: {} have found", val);
  }
  return std::nullopt;
}

std::optional<std::string> SemanticAnalyzer::visit(BinaryOpASTNode& node) {
}

std::optional<std::string> SemanticAnalyzer::visit(UnaryOpASTNode& node) {

}

std::optional<std::string> SemanticAnalyzer::visit(FuncCallASTNode& node) {

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

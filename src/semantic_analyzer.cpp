#include "semantic_analyzer.h"
#include "ast_node.h"
#include "expression_ast.h"
#include "statement_ast.h"
#include "symbol_info.h"
#include "token.h"

#include <iterator>
#include <memory>
#include <format>
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer() : 
  main_scope_(std::make_shared<Scope>()),
  current_scope_(main_scope_) {
}

std::vector<std::pair<std::string, std::size_t>> 
  SemanticAnalyzer::analyze(std::vector<std::unique_ptr<StatementAST>> program) {
  std::cout << "UNIMPLEMENTED";
  return std::vector<std::pair<std::string, std::size_t>>{};
  // main loop, don't know what to with it lmaooo
}

std::pair<std::string, std::size_t> SemanticAnalyzer::check_scope() {
  std::cout << "UNIMPLEMENTED";
  return {"", 0};
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
  std::string identifier_name = node.get_val();
  std::shared_ptr<Scope> iter_scope = current_scope_;

  while (iter_scope != nullptr && 
      iter_scope->symbols.find(identifier_name)->second.name != "FunctionDeclaration") {
    iter_scope = iter_scope->parent;
  }

  if (iter_scope == nullptr) {
    return std::format("No variable declaration found in name {}", identifier_name);
  }

  ASTNode* found_id_decl = nullptr;
  if (found_id_decl = declarations_[&iter_scope->symbols[identifier_name]], found_id_decl == nullptr) {
    return std::format("No variable declaration found in declarations {}", identifier_name);
  }

  VariableDeclarationASTNode* id_decl = nullptr;
  if (id_decl = dynamic_cast<VariableDeclarationASTNode*>(found_id_decl), id_decl == nullptr) {
    return std::format("Variable declaration found but it's not in appropriate form");
  }

  if (id_decl->get_name() != node.get_val()) {
    return std::format("I don't know what the fucking hell is this, I'm just writing code, fuck me");
  }

  // insert type so the semantic analyzer can infer the type of identifier
  semantic_info_.set_symbol(&node, new SymbolInfo("Identifier", id_decl->get_token())); 
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
  
  ASTNode* found_func_decl = nullptr;
  if (found_func_decl = declarations_[&iter_scope->symbols[func_name]], found_func_decl == nullptr) {
    return std::format("No function declaration found in declarations {}", node.get_func_call_name());
  }

  FunctionDefASTNode* res = dynamic_cast<FunctionDefASTNode*>(found_func_decl);
  if (res != nullptr) {
    std::vector<FunctionDefASTNode::Parameter> found_params = res->get_parameters();
    std::vector<ExpressionAST*> node_params = node.get_arguments();

    if (found_params.size() != node_params.size()) {
      return std::format("Found function declaration doesn't match the function call");
    }

    auto it_found_params = found_params.begin();
    auto it_node_args = node.get_arguments().begin();
    
    for (; it_found_params != found_params.end(); it_found_params++) {
      it_node_args = node.get_arguments().begin() + std::distance(found_params.begin(), it_found_params);
      (*it_node_args)->accept(*this);

      const SymbolInfo* arg_info = semantic_info_.get_symbol((*it_node_args));

      if ((it_found_params->name != arg_info->name) || (it_found_params->type != arg_info->token)) {
        return std::format("Found function declaration doesn't match parameter list {}", it_found_params->name);
      }
    }

  }
  else {
    return std::format("Found symbol is not declaration {}", node.get_func_call_name());
  }

  return std::nullopt;
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
  return std::format("Unimplemented");
}

std::optional<std::string> SemanticAnalyzer::visit(VariableDeclarationASTNode& node) {
  return std::format("Unimplemented");
}

std::optional<std::string> SemanticAnalyzer::visit(AssignmentASTNode& node) {
  return std::format("Unimplemented");
}

std::optional<std::string> SemanticAnalyzer::visit(IfStatementASTNode& node) {
  return std::format("Unimplemented");
}

std::optional<std::string> SemanticAnalyzer::visit(WhileStatementASTNode& node) {
  return std::format("Unimplemented");
}

std::optional<std::string> SemanticAnalyzer::visit(ForStatementASTNode& node) {
  return std::format("Unimplemented");
}

std::optional<std::string> SemanticAnalyzer::visit(ReturnStatementASTNode& node) {
  return std::format("Unimplemented");
}

std::optional<std::string> SemanticAnalyzer::visit(FunctionDefASTNode& node) {
  return std::format("Unimplemented");
}

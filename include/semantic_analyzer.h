#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "expression_ast.h"
#include "statement_ast.h"
#include "symbol_info.h"
#include "semantic_info.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

class StatementAST;
class ExpressionAST;

struct Scope {
  std::shared_ptr<Scope> parent {nullptr};
  std::unordered_map<std::string, SymbolInfo> symbols;
};

class SemanticAnalyzer {
public:
  SemanticAnalyzer();
  ~SemanticAnalyzer() = default;
  // vector of errors containing error string and line
  std::vector<std::pair<std::string, std::size_t>> analyze(std::vector<std::unique_ptr<StatementAST>> program);
  // lookup for the symbol in current and parent scopes
  std::pair<std::string, std::size_t> check_scope();

  // Expression visitors
  virtual std::optional<std::string> visit(IntASTNode& node);
  virtual std::optional<std::string> visit(FloatASTNode& node);
  virtual std::optional<std::string> visit(StringASTNode& node);
  virtual std::optional<std::string> visit(IdentifierASTNode& node);
  virtual std::optional<std::string> visit(BinaryOpASTNode& node);
  virtual std::optional<std::string> visit(UnaryOpASTNode& node);
  virtual std::optional<std::string> visit(FuncCallASTNode& node);

  // statement visitors
  virtual std::optional<std::string> visit(ExpressionStatementASTNode& node);
  virtual std::optional<std::string> visit(VariableDeclarationASTNode& node);
  virtual std::optional<std::string> visit(AssignmentASTNode& node);
  virtual std::optional<std::string> visit(IfStatementASTNode& node);
  virtual std::optional<std::string> visit(WhileStatementASTNode& node);
  virtual std::optional<std::string> visit(ForStatementASTNode& node);
  virtual std::optional<std::string> visit(ReturnStatementASTNode& node);
  virtual std::optional<std::string> visit(FunctionDefASTNode& node);
private:
  bool is_compatible(const Token& t1, const Token& t2);
  Token infer_result_type(const Token operand, const Token& lhs, const Token& rhs);
  // keep track of what scope I'm currently in
  std::shared_ptr<Scope> main_scope_;
  std::shared_ptr<Scope> current_scope_;
  
  std::unordered_map<SymbolInfo*, ASTNode*> declarations_;

  SemanticInfo semantic_info_;
};



#endif

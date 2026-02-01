#include "statement_ast.h"
#include "semantic_analyzer.h"

void ExpressionStatementASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void VariableDeclarationASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void AssignmentASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void IfStatementASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void WhileStatementASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void ForStatementASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void ReturnStatementASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void FunctionDefASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

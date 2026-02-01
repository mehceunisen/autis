#include "expression_ast.h"
#include "semantic_analyzer.h"

void IntASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void FloatASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void StringASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void IdentifierASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void BinaryOpASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void UnaryOpASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

void FuncCallASTNode::accept(SemanticAnalyzer& analyzer) {
  analyzer.visit(*this);
}

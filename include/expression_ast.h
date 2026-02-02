#ifndef EXPRESSION_AST_H
#define EXPRESSION_AST_H

#include <algorithm>
#include <vector>

#include "ast_node.h"

class SemanticAnalyzer;

class IntASTNode : public ExpressionAST {
public:
  IntASTNode(int64_t v) : val(v) {}
  int64_t get_val() const { return val; };
  void accept(SemanticAnalyzer& analyzer) override;
private:
  int64_t val;
};

class FloatASTNode : public ExpressionAST {
public:
  FloatASTNode(float v) : val(v) {}
  float get_val() const { return val; };
  void accept(SemanticAnalyzer& analyzer) override;
private:
  float val;
};

class StringASTNode : public ExpressionAST {
public:
  StringASTNode(std::string v) : val(std::move(v)) {}
  const std::string& get_val() const { return val; };
  void accept(SemanticAnalyzer& analyzer) override;
private:
  std::string val;
};

class IdentifierASTNode : public ExpressionAST {
public:
  IdentifierASTNode(std::string v) : val(std::move(v)) {}
  const std::string& get_val() const { return val; } 
  void accept(SemanticAnalyzer& analyzer) override;
private:
  std::string val;
};

class BinaryOpASTNode : public ExpressionAST {
public:
  BinaryOpASTNode(Token _op_token, std::unique_ptr<ExpressionAST> _lhs, std::unique_ptr<ExpressionAST> _rhs) 
    : op_token(_op_token), lhs(std::move(_lhs)), rhs(std::move(_rhs)) {}
  void accept(SemanticAnalyzer& analyzer) override;

  ExpressionAST& get_lhs() { return *lhs.get(); }
  ExpressionAST& get_rhs() { return *rhs.get(); }
  Token& get_operand() { return op_token; }
private: 
  Token op_token;
  std::unique_ptr<ExpressionAST> lhs, rhs;
};

class UnaryOpASTNode : public ExpressionAST {
public:
  UnaryOpASTNode(Token _op_token, std::unique_ptr<IdentifierASTNode> _identifier) 
    : op_token(_op_token), identifier(std::move(_identifier)) {}
  void accept(SemanticAnalyzer& analyzer) override;
  Token& get_operator_token() { return op_token; }
  IdentifierASTNode& get_identifier() { return *identifier.get(); }
private:
  Token op_token;
  std::unique_ptr<IdentifierASTNode> identifier;
};

class FuncCallASTNode : public ExpressionAST {
public:
  FuncCallASTNode(std::string _name, std::vector<std::unique_ptr<ExpressionAST>> _arguments) 
    : name(_name), arguments(std::move(_arguments)) {}
  void accept(SemanticAnalyzer& analyzer) override;
  std::string& get_func_call_name() { return name; }

  std::vector<ExpressionAST*> get_arguments() {
    std::vector<ExpressionAST*> ret; // TODO(MehceUnisen): violating memory safety!
    for (auto& arg: arguments) {
      ret.emplace_back(arg.get());
    }
    return ret;
  }
private:
  std::string name;
  std::vector<std::unique_ptr<ExpressionAST>> arguments;
};

#endif

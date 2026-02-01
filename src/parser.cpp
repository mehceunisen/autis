#include "parser.h"
#include "ast_node.h"
#include "lexer.h"
#include "expression_ast.h"
#include "statement_ast.h"
#include "token.h"
#include "ast_util.h"

#include <iostream>
#include <iterator>
#include <stdexcept>
#include <memory>

Parser::Parser(Lexer& lexer) : lexer_(lexer) {
  current_lxm_ = lexer_.get_lexeme();

  if (current_lxm_.token == Undefined ||
      current_lxm_.token == EndOfFile) 
  {
    throw std::runtime_error("somethings off dude\n");
  }
}

Lexeme Parser::advance_lexeme() {
  prev_lxm_ = current_lxm_; // cache last lexeme 
  Lexeme lxm = lexer_.get_lexeme(); // get new lexeme
  Lexeme tmp = current_lxm_; // store popped lexeme
  current_lxm_ = lxm; // move to next lexeme
  return tmp; // return popped lexeme
}

std::unique_ptr<StatementAST> Parser::parse() {
  if (type_set.contains(current_lxm_.token)) {
    return parse_variable_declaration();
  }
  else if (current_lxm_.token == Token::Identifier) {
    if (lexer_.peek_next_lexeme().token == ParanOpen) {
      return std::make_unique<ExpressionStatementASTNode>
        (parse_function_call());
    }
    else if (lexer_.peek_next_lexeme().token == Token::OpAssign) {
      Lexeme name = advance_lexeme();
      advance_lexeme(); // eat name
      return std::make_unique<AssignmentASTNode>
        (name.raw_val, parse_binary_op());
    }
    return std::make_unique<ExpressionStatementASTNode>(parse_identifier_expression());
  }
  else if (current_lxm_.token == Token::FuncKword) {
    if (lexer_.peek_next_lexeme().token == Identifier) {
      return parse_function_def();
    }
  }
  else if (current_lxm_.token == Token::FuncRet) {
    return parse_return_statement();
  }
  else if (current_lxm_.token == EndOfLine) {
    advance_lexeme();
    return parse();
  }
  
  return nullptr;
}

std::unique_ptr<StatementAST> Parser::parse_variable_declaration() {
  Lexeme type = advance_lexeme(); // eat Type

  if (current_lxm_.token != Token::Identifier)
    throw std::runtime_error(
        std::format("expected identifier at line {}", 
          lexer_.get_current_line())); 
  Lexeme identifier = advance_lexeme();


  std::unique_ptr<ExpressionAST> binary_op {nullptr};
  if (current_lxm_.token == Token::OpAssign) { // we have a rhs
    advance_lexeme(); // eat =

    binary_op = parse_binary_op();

    if (binary_op == nullptr)
      throw std::runtime_error(
          std::format("expected literal at line {}", 
            lexer_.get_current_line()));
  }
  advance_lexeme(); // eat \n
  return std::make_unique<VariableDeclarationASTNode>
    (identifier.raw_val, type.token, std::move(binary_op));
}

std::unique_ptr<ExpressionAST> Parser::parse_unary_expression() {
  Lexeme identifier = advance_lexeme(); // eat identifier 
  Lexeme operand = advance_lexeme(); // eat end operand

  return std::make_unique<UnaryOpASTNode>(operand.token,
      std::make_unique<IdentifierASTNode>(identifier.raw_val));

}

std::unique_ptr<ExpressionAST> Parser::parse_identifier_expression() {
  Lexeme next_lxm = lexer_.peek_next_lexeme();
  if (next_lxm.token == OpInc || next_lxm.token == OpDec) {
    return parse_unary_expression();
  }


  else if (operator_set.contains(next_lxm.token) || 
      next_lxm.token == EndOfLine) {
    Lexeme identifier = advance_lexeme(); // eat identifier
    return std::make_unique<IdentifierASTNode>(identifier.raw_val);
  }

  throw std::runtime_error(
      std::format("expected identifier at line {}", 
        lexer_.get_current_line())); 
}

std::unique_ptr<ExpressionAST> Parser::parse_binary_op() {
  std::unique_ptr<ExpressionAST> lhs = unique_ptr_cast<ExpressionAST>(parse_primary()); 
  return parse_binary_op_rhs(0, std::move(lhs));

}

std::unique_ptr<ExpressionAST> Parser::parse_binary_op_rhs(int exper_prec, std::unique_ptr<ExpressionAST> lhs) {
  while (true) {
    if(!operator_set.contains(current_lxm_.token) ||
        binop_precedence.at(current_lxm_.token) < exper_prec) {
      return lhs;
    }

    int next_prec = 0;
    if (operator_set.contains(current_lxm_.token)) {
      next_prec = binop_precedence.at(current_lxm_.token);
    }

    Lexeme op = advance_lexeme(); // eat current op
    std::unique_ptr<ExpressionAST> rhs = parse_primary();
    if (next_prec > exper_prec) {
      rhs = parse_binary_op_rhs(binop_precedence[op.token] + 1, std::move(rhs));
    }
    lhs = std::make_unique<BinaryOpASTNode>(op.token, std::move(lhs), std::move(rhs));
  } 
}

std::unique_ptr<ExpressionAST> Parser::parse_primary() {
  if (literal_set.contains(current_lxm_.token)) {
    return parse_literal(); // function will eat token
  }
  else if (current_lxm_.token == Token::Identifier) {
    // eat token
    return parse_identifier_expression();
  }
  else if (current_lxm_.token == Token::FuncKword) {
    return parse_function_call();
  }

  return nullptr;
}

// TODO: this could be improved with a design pattern
std::unique_ptr<ExpressionAST> Parser::parse_literal() { 
  if (current_lxm_.token == IntLiteral) {
    Lexeme ret = advance_lexeme(); // eat int literal
    return std::make_unique<IntASTNode>(std::stol(ret.raw_val));
  }
  else if (current_lxm_.token == FloatLiteral) {
    Lexeme ret = advance_lexeme(); // eat float literal
    return std::make_unique<FloatASTNode>(std::stof(ret.raw_val));
  }
  else if (current_lxm_.token == StringLiteral) {
    Lexeme ret = advance_lexeme();
    return std::make_unique<StringASTNode>(ret.raw_val);
  }
  else {
    return nullptr;
  }
}

std::unique_ptr<StatementAST> Parser::parse_function_def() {
  advance_lexeme();  // eat fn keyword
  Lexeme func_name = advance_lexeme(); // eat func_name
  if (func_name.token != Identifier) {
    throw std::runtime_error(
        std::format("Expected function name at line {}",
          lexer_.get_current_line()));
  }

  if (advance_lexeme().token != ParanOpen) { // eat and look
    throw std::runtime_error(
        std::format("Expected '(' at line {}",
          lexer_.get_current_line()));
  }

  std::vector<FunctionDefASTNode::Parameter> parameters;
  while (current_lxm_.token != ParanClose) { // if parameter list is not empty
                                             //()
                                             //(x:int32)
                                             //(x:int32, y:uint32)
                                             //id_name : var_type
    Lexeme param_name = advance_lexeme();
    if (param_name.token != Identifier) {
      throw std::runtime_error(
          std::format("Expected identifier at line {}",
            lexer_.get_current_line()));
    }

    if (advance_lexeme().token != Colon) {
      throw std::runtime_error(
          std::format("Expected colon at line {}",
            lexer_.get_current_line()));
    }

    Lexeme param_type = advance_lexeme();
    if (!type_set.contains(param_type.token)) {
      throw std::runtime_error(
          std::format("Expected variable type at line {}",
            lexer_.get_current_line()));
    }
    parameters.emplace_back(param_type.token, param_name.raw_val);

    if (current_lxm_.token == Token::Comma) { 
      // if there are more then one parameters, expect comma (,)
      advance_lexeme(); // eat comma (,)
    }
  }

  if (current_lxm_.token != ParanClose) { 
    throw std::runtime_error(
        std::format("Expected ')' at line {}",
          lexer_.get_current_line()));
  }

  advance_lexeme(); // eat )
  if (current_lxm_.token != Colon) {
    throw std::runtime_error(
        std::format("Expected : at line {}",
          lexer_.get_current_line()));
  }

  advance_lexeme(); // consume colon

  if (!type_set.contains(current_lxm_.token)) {
    throw std::runtime_error(
        std::format("Expected return type at line {}",
          lexer_.get_current_line()));
  }

  Lexeme ret_type = advance_lexeme();

  std::vector<std::unique_ptr<StatementAST>> func_body;

  if (current_lxm_.token != Token::CurBraceOpen) {
    throw std::runtime_error(
        std::format("Expected closure at line {}",
          lexer_.get_current_line()));
  }
  advance_lexeme(); // eat {
  if (advance_lexeme().token != Token::EndOfLine) { // eat \n
    throw std::runtime_error(
        std::format("Expected new line at line {}",
          lexer_.get_current_line()));
  }

  while (current_lxm_.token != Token::CurBraceClose && current_lxm_.token != EndOfFile) {
    func_body.emplace_back(parse());
    if (current_lxm_.token == Token::EndOfLine) {
      advance_lexeme(); // eat
    }
  }

  if (current_lxm_.token == EndOfFile) {
    throw std::runtime_error(
        std::format("Expected CurlyBraceClose at line {}",
          lexer_.get_current_line()));
  }
  advance_lexeme(); // eat }

  return std::make_unique<FunctionDefASTNode>(func_name.raw_val, 
      std::move(parameters), ret_type.token,
      std::move(func_body));
}

std::unique_ptr<ExpressionAST> Parser::parse_function_call() {
  Lexeme func_name = advance_lexeme(); //eat func name
  std::vector<std::unique_ptr<ExpressionAST>> args;

  if (advance_lexeme().token != ParanOpen) {
    throw std::runtime_error(
        std::format("Expected '(' at line {}",
          lexer_.get_current_line()));
  } // eat paran open

  if (prev_lxm_.token == ParanOpen && current_lxm_.token != ParanClose) {
    // if parameter list is not empty
    while(prev_lxm_.token != ParanClose) {
      args.emplace_back(parse_binary_op());
      advance_lexeme(); // eat comma
    }

    if (prev_lxm_.token != ParanClose) { 
      throw std::runtime_error(
          std::format("Expected ')' at line {}",
            lexer_.get_current_line()));
    }
  }

  return std::make_unique<FuncCallASTNode>
    (func_name.raw_val, std::move(args));
}

std::unique_ptr<StatementAST> Parser::parse_return_statement() {
  advance_lexeme(); // eat "return"
  auto ret = parse_primary();
  advance_lexeme(); // eat retval
  return std::make_unique<ExpressionStatementASTNode>(std::move(ret));
}


#include "parser.h"
#include "lexer.h"
#include "expression_ast.h"
#include "statement_ast.h"
#include "token.h"


#include <iostream>
#include <iterator>

Parser::Parser(Lexer& lexer) : lexer_(lexer) {
    current_lxm_ = lexer_.get_lexeme();
    next_lxm_ = lexer_.get_lexeme();

    if (current_lxm_.token == Undefined ||
            current_lxm_.token == EndOfFile|| 
            next_lxm_.token == Undefined) 
    {
        throw std::runtime_error("somethings off dude\n");
    }
}

Lexeme Parser::advance_lexeme() {
    Lexeme lxm = lexer_.get_lexeme();

    Lexeme tmp = current_lxm_;
    current_lxm_ = next_lxm_;
    next_lxm_ = lxm;

    return tmp;
}

ASTNode* Parser::parse() { 
    // assignment, binary and unary operation, fn decl, fn call
    
    // we will act according to type of current_lxm_
    if (type_set.contains(current_lxm_.token)) { // this is an assignment
            Lexeme type = advance_lexeme(); // eat Type
            if (current_lxm_.token != Token::Identifier)
                throw std::runtime_error(
                std::format("expected identifier at line {}", 
                    lexer_.get_current_line())); 
            Lexeme identifier = advance_lexeme();
            if (current_lxm_.token != Token::OpAssign)
                throw std::runtime_error(
                std::format("expected assignment at line {}", 
                    lexer_.get_current_line())); 
            advance_lexeme(); // eat equal sign
                              
            ExpressionAST* binary_op = parse_binary_op(); 
            if (binary_op == nullptr)
                throw std::runtime_error(
                std::format("expected literal at line {}", 
                    lexer_.get_current_line()));
            //Lexeme eol = advance_lexeme();
            //if (eol.token != Token::EndOfLine) 
            //    throw std::runtime_error(std::format
            //            ("no expression expected at the end of line {}", 
            //        lexer_.get_current_line()));
            return new AssignmentASTNode(identifier.raw_val, binary_op);
    }
}

ExpressionAST* Parser::parse_primary() {
    if (literal_set.contains(current_lxm_.token)) {
        return parse_literal(); // function will eat token
    }
    else if (current_lxm_.token == Token::Identifier) {
        // eat token
        return new IdentifierASTNode(advance_lexeme().raw_val);
    }
}

ExpressionAST* Parser::parse_binary_op() {
    ExpressionAST* lhs = parse_primary();
    Lexeme op = advance_lexeme();
    if (op.token == Token::EndOfLine) {
        return lhs;
    }
    if (!operator_set.contains(op.token)) {
        throw std::runtime_error(
                std::format("expected binary operator at line {}",
                    lexer_.get_current_line())); 
    }
    return new BinaryOpASTNode(op.token, lhs,
            parse_binary_op());
}

// this could be improved with 
// a design pattern
ExpressionAST* Parser::parse_literal() { 
    if (current_lxm_.token == IntLiteral) {
        Lexeme ret = advance_lexeme(); // eat int literal
        return new IntASTNode(std::stol(ret.raw_val));
    }
    else if (current_lxm_.token == FloatLiteral) {
        Lexeme ret = advance_lexeme(); // eat float literal
        return new FloatASTNode(std::stof(ret.raw_val));
    }
    else if (current_lxm_.token == StringLiteral) {
        Lexeme ret = advance_lexeme();
        return new StringASTNode(ret.raw_val);
    }
    else {
        return nullptr;
    }
}


#include "parser.h"
#include "ast_node.h"
#include "lexer.h"
#include "expression_ast.h"
#include "statement_ast.h"
#include "token.h"
#include "ast_util.h"

#include <iostream>
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
    Lexeme lxm = lexer_.get_lexeme();
    Lexeme tmp = current_lxm_;
    current_lxm_ = lxm;
    return tmp;
}

std::unique_ptr<ASTNode> Parser::parse() { 
    // assignment, binary and unary operation, fn decl, fn call
    
    // we will act according to type of current_lxm_
    if (type_set.contains(current_lxm_.token)) { // this is an assignment
            advance_lexeme(); // eat Type
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
                              
            std::unique_ptr<ExpressionAST> binary_op = parse_binary_op(); 
            if (binary_op == nullptr)
                throw std::runtime_error(
                std::format("expected literal at line {}", 
                    lexer_.get_current_line()));
            
            return std::make_unique<AssignmentASTNode>(identifier.raw_val, std::move(binary_op));
    }
    
    if (current_lxm_.token == FuncDef) { // this is a function definition
        advance_lexeme();  // eat fn keyword
        return parse_function_def();
    }

    return nullptr;
}


std::unique_ptr<ExpressionAST> Parser::parse_binary_op() {
    // TODO: ensure type safety    
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
        std::unique_ptr<ExpressionAST> rhs = unique_ptr_cast<ExpressionAST>(parse_primary());
        if (next_prec > exper_prec) {
            rhs = parse_binary_op_rhs(binop_precedence[op.token] + 1, std::move(rhs));
        }
        lhs = std::make_unique<BinaryOpASTNode>(op.token, std::move(lhs), std::move(rhs));
    } 
}


std::unique_ptr<ASTNode> Parser::parse_primary() {
    if (literal_set.contains(current_lxm_.token)) {
        return parse_literal(); // function will eat token
    }
    else if (current_lxm_.token == Token::Identifier) {
        // eat token
        return std::make_unique<IdentifierASTNode>(advance_lexeme().raw_val);
    }
    return nullptr;
}

// this could be improved with 
// a design pattern
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
    // current lexeme should be function name 
    Lexeme func_name = advance_lexeme();
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

    using Parameter = FunctionDefASTNode::Parameter;

    std::vector<Parameter> parameters;
    Lexeme prev_lexm;
    if (current_lxm_.token != ParanClose) { // if parameter list is not empty
        //()
        //(x:int32)
        //(x:int32, y:uint32)
        do {
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
            prev_lexm = advance_lexeme();
        } while(prev_lexm.token == Comma && prev_lexm.token != ParanClose);
        
    }
     
    if (prev_lexm.token != ParanClose) { 
        throw std::runtime_error(
                std::format("Expected ')' at line {}",
                    lexer_.get_current_line()));
    }

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
    
    //TODO:parse body

    return nullptr;
}

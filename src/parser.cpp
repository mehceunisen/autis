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
    // TODO: add prefix unary
    Lexeme identifier = advance_lexeme(); // eat identifier 
    Lexeme operand = advance_lexeme(); // eat end operand
                                       
    return std::make_unique<UnaryOpASTNode>(operand.token,
            std::make_unique<IdentifierASTNode>(identifier.raw_val));

}

std::unique_ptr<StatementAST> Parser::parse_identifier_statement() {
    Lexeme next_lxm = lexer_.peek_next_lexeme();
    if (next_lxm.token == OpInc || next_lxm.token == OpDec) {
        // TODO: add prefix unary
        return std::make_unique<ExpressionStatementASTNode>(
                parse_unary_expression());
    }

    else if (next_lxm.token == Token::OpAssign) {
        advance_lexeme(); // eat equal sign
        Lexeme name = advance_lexeme();

        return std::make_unique<AssignmentASTNode>
            (name.raw_val, parse_binary_op());
    }

    else if (operator_set.contains(next_lxm.token) || 
            next_lxm.token == EndOfLine) {
        Lexeme identifier = advance_lexeme(); // eat identifier
        return std::make_unique<ExpressionStatementASTNode>(std::make_unique<IdentifierASTNode>(identifier.raw_val));
    }

    throw std::runtime_error(
            std::format("expected identifier at line {}", 
                lexer_.get_current_line())); 
}

std::unique_ptr<ASTNode> Parser::my_parse() {
    if (type_set.contains(current_lxm_.token)) {
        return parse_variable_declaration();
    }
    else if (current_lxm_.token == Token::Identifier) {
        return parse_identifier_statement();
    }
    else if (current_lxm_.token == Token::FuncCall) {
        return parse_function_call();
    }
    else if (current_lxm_.token == Token::FuncDef) {
        return parse_function_def();
    }

    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parse() { 
    // assignment, binary and unary operation, fn decl, fn call
    // we will act according to type of current_lxm_
    if (type_set.contains(current_lxm_.token)) { // this is var decl 
        Lexeme type = advance_lexeme(); // eat Type
        if (current_lxm_.token != Token::Identifier)
            throw std::runtime_error(
                    std::format("expected identifier at line {}", 
                        lexer_.get_current_line())); 
        Lexeme identifier = advance_lexeme();
        

        std::unique_ptr<ExpressionAST> binary_op {nullptr};
        if (current_lxm_.token == Token::OpAssign) { // we have a rhs
            advance_lexeme(); // eat equal sign
            binary_op = parse_binary_op(); 
            if (binary_op == nullptr)
                throw std::runtime_error(
                        std::format("expected literal at line {}", 
                            lexer_.get_current_line()));
        }
        advance_lexeme(); // eat end of line
        return std::make_unique<VariableDeclarationASTNode>
        (identifier.raw_val, type.token, std::move(binary_op));
        }

    else if (current_lxm_.token == Token::Identifier) { //unary, asgn
        Lexeme identifier = advance_lexeme(); // eat identifier token
        if (current_lxm_.token == OpInc || current_lxm_.token == OpDec) {
            // TODO: add prefix unary
            Lexeme operand = advance_lexeme(); // eat operator 
            if (current_lxm_.token != Token::EndOfLine) {
                throw std::runtime_error(
                        std::format("expected end of line at line {}", 
                            lexer_.get_current_line()));
            }
            advance_lexeme(); // eat end of line
            return std::make_unique<UnaryOpASTNode>
                (operand.token,
                 std::make_unique<IdentifierASTNode>(identifier.raw_val));
        }
        else if (current_lxm_.token == Token::OpAssign) {
            advance_lexeme(); // eat equal sign
            Lexeme name = advance_lexeme();
            if (current_lxm_.token == FuncCall) {
                return parse_function_call();
            }

            return std::make_unique<AssignmentASTNode>
                (name.raw_val, parse_binary_op());
        }
    }
    
    else if (current_lxm_.token == FuncCall) {
        return parse_function_call();
    } 

    else if (current_lxm_.token == FuncDef) { // this is a function definition
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
        return parse_identifier_statement();
    }
    else if (current_lxm_.token == Token::FuncCall) {
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
    if (func_name.token != FuncCall) {
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
        } while(prev_lxm_.token == Comma && prev_lxm_.token != ParanClose);
        
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
    
    //TODO:parse body
    std::vector<std::unique_ptr<StatementAST>> func_body;
    
    return std::make_unique<FunctionDefASTNode>(func_name.raw_val, 
            parameters, ret_type.token,
            std::move(func_body));
}

std::unique_ptr<ExpressionAST> Parser::parse_function_call() {
    Lexeme func_name = advance_lexeme(); //eat func name
    std::vector<std::unique_ptr<ExpressionAST>> args;

    Lexeme prev_lxm = advance_lexeme(); // eat paran open
    if (prev_lxm.token == ParanOpen && current_lxm_.token != ParanClose) {
        // if parameter list is not empty
        while(prev_lxm.token != ParanClose) {
            // TODO: this doesn't support unary args
            args.emplace_back(parse_binary_op());
            prev_lxm = advance_lexeme();
        }
     
        if (prev_lxm.token != ParanClose) { 
            throw std::runtime_error(
                    std::format("Expected ')' at line {}",
                        lexer_.get_current_line()));
        }
    }
    return std::make_unique<FuncCallASTNode>
        (func_name.raw_val, std::move(args));
}

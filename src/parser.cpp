#include "parser.h"
#include "lexer.h"
#include "expression_ast.h"
#include "statement_ast.h"


#include <iostream>

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
    // we will act according to type of current_lxm_
    if (type_set.contains(current_lxm_.token)) {
            ASTNode* primary = parse_literal();
            Lexeme type = advance_lexeme(); // eat primary
            if (current_lxm_.token != Token::Identifier)
                throw std::runtime_error(
                std::format("expected identifier at line {}", 
                    lexer_.get_current_line())); 
            Lexeme identifier = advance_lexeme();
            if (current_lxm_.token != Token::OpAssign)
                throw std::runtime_error(
                std::format("expected assignment at line {}", 
                    lexer_.get_current_line())); 
            Lexeme op = advance_lexeme(); // eat equal sign
            ExpressionAST* val = parse_literal(); 
            // TODO:need to check is val and primary are same type
            if (!val)
                throw std::runtime_error(
                std::format("expected literal at line {}", 
                    lexer_.get_current_line()));
            // whole assignment has been parsed. gotta check whether 
            // that's the end of line
            Lexeme eol = advance_lexeme();
            if (eol.token != Token::EndOfLine) 
                throw std::runtime_error(std::format
                        ("no expression expected at the end of line {}", 
                    lexer_.get_current_line()));

            return new AssignmentASTNode(identifier.raw_val, val);
    }
}

// this could be imporved with 
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

ExpressionAST* Parser::parse_operator(ASTNode* lhs) {
    //Lexeme op_lexm = advance_lexeme(); // consume + operator
    //ASTNode* rhs = parse();
    //if (rhs == nullptr) {
    //    return new OperatorASTNode(op_lexm.token, lhs, nullptr);
    //}
    //return new OperatorASTNode(op_lexm.token, lhs, rhs);
}

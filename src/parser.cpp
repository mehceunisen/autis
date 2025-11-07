#include "parser.h"
#include "expression_ast.h"
#include "lexer.h"


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
    //ASTNode* primary = parse_literal();
    //
    //if (operator_set.contains(current_lxm_.token)) {
    //    return parse_operator(primary);
    //}
    //else {
    //    return primary;
    //}
}

// this could be imporved with 
// a design pattern
ExpressionAST* Parser::parse_literal() { 
   // if (current_lxm_.token == IntLiteral) {
   //     Lexeme ret = advance_lexeme(); // eat int literal
   //     return new IntASTNode(std::stol(ret.raw_val));
   // }
   // else if (current_lxm_.token == FloatLiteral) {
   //     Lexeme ret = advance_lexeme(); // eat float literal
   //     return new FloatASTNode(std::stof(ret.raw_val));
   // }
   // else if (current_lxm_.token == StringLiteral) {
   //     Lexeme ret = advance_lexeme();
   //     return new StringASTNode(ret.raw_val);
   // }
   // else {
   //     return nullptr;
   // }
}

ExpressionAST* Parser::parse_operator(ASTNode* lhs) {
    //Lexeme op_lexm = advance_lexeme(); // consume + operator
    //ASTNode* rhs = parse();
    //if (rhs == nullptr) {
    //    return new OperatorASTNode(op_lexm.token, lhs, nullptr);
    //}
    //return new OperatorASTNode(op_lexm.token, lhs, rhs);
}

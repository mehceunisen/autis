#include "parser.h"
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

Lexeme Parser::push_lexeme(Lexeme lxm) {
    Lexeme tmp = current_lxm_;
    current_lxm_ = next_lxm_;
    next_lxm_ = lxm;
    return tmp;
}

void Parser::parse() {
    auto st = parse_statement();
    std::cout << dynamic_cast<StatementAST*>(st.get())->val; 

    if (current_lxm_.token == OpAdd) {
                
        auto exp = parse_expression(std::move(st));
    
    }
}

std::unique_ptr<ASTNode> Parser::parse_statement() {
    if (current_lxm_.token == IntLiteral) {
        Lexeme ret = push_lexeme(lexer_.get_lexeme());
        std::cout << "raw val" << ret.raw_val << "\n";
        return std::make_unique<StatementAST>(
                std::stoi(ret.raw_val));
    }
    else {
        return nullptr;
    }
}

std::unique_ptr<ASTNode> 
Parser::parse_expression(std::unique_ptr<ASTNode> lhs) {
    if (current_lxm_.token == OpAdd) {
        std::cout << "i came in\n";
        Lexeme ret = push_lexeme(lexer_.get_lexeme()); // ate  +
        auto _rhs = parse_statement();
        if (_rhs) {
            _rhs = std::move(parse_expression(std::move(_rhs))); 
        }
        std::cout << dynamic_cast<StatementAST*>(lhs.get())->val + dynamic_cast<StatementAST*>(_rhs.get())->val;
    }
    return nullptr;
}

#include "lexer.h"
#include <cctype>
#include <iostream>
#include <stdexcept>

Lexer::Lexer(std::string file_path) {

    std::fstream file(file_path);
    std::stringstream buffer;

    buffer << file.rdbuf();
    source_ = buffer.str();
}

int isop(char ch) {
    return std::string{"+-/*<>=!"}.find(ch) != std::string::npos;
}

char Lexer::advance() {
    if (last_idx_ > source_.size())
        return '\0';

    return source_[last_idx_++];
}

char Lexer::peek() {
    if (last_idx_ >= source_.size())
        return '\0';

    return source_[last_idx_];
}

char Lexer::peek_next() {
    if (last_idx_ + 1 >= source_.size())
        return '\0';

    return source_[last_idx_ + 1];
}

Lexeme Lexer::get_lexeme() {
    std::string word;

    while (peek() == ' ')
        advance();

    char ch = peek();

    if (std::isalpha(ch)) {
        do {
            word += advance();
        } while(std::isalnum(peek()) || peek() == '_');
        
        if (lexeme_lookup_table.contains(word)) {
            return lexeme_lookup_table[word];
        }

        return Lexeme(Identifier, word);
    }

    else if (std::isdigit(ch)) {
        do {
            word += advance();
        } while(std::isdigit(peek()) || peek() == '.');
        
        if (word.find('.') != std::string::npos)
            return Lexeme(FloatLiteral, word);
        
        return Lexeme(IntLiteral, word);
    }

    else if (isop(ch)) {
        word += advance();
        auto next = peek();

        if (ch == '+' && next == '+')
            word += advance();
        else if (ch == '-' && next == '-')
            word += advance();
        else if (ch == '=' && next == '=')
            word += advance();
        else if ((ch == '<' || ch == '>') && next == '=')
            word += advance();
        else if (ch == '!' && next == '=')
            word += advance();

        return lexeme_lookup_table.at(word);
    } else if (ch == '(') {
        word += advance();
        return lexeme_lookup_table.at(word);
    }
    else if (ch == ')') {
        word += advance();
        return lexeme_lookup_table.at(word);
    }
    else if (ch == '{') {
        word += advance();
        return lexeme_lookup_table.at(word);
    }
    else if (ch == '}') {
        word += advance();
        return lexeme_lookup_table.at(word);
    }
    else if (ch == ':') {
        word += advance();
        return lexeme_lookup_table.at(word);
    }
    else if (ch == ',') {
        word += advance();
        return lexeme_lookup_table.at(word);
    }
    else if (ch == '\n') {
        word += advance();
        current_line_++;
        return Lexeme(EndOfLine, std::string{"\\n"});
    }
    else if (ch == '\"') {
        advance();
        while(peek() != '\"'){
            word += advance();
        }
        advance();
        return Lexeme(StringLiteral, word);
    }
    else if (last_idx_ == source_.length()) {
        return Lexeme(EndOfFile, "");
    }

    return Lexeme(Undefined, "");
}

Lexeme Lexer::peek_next_lexeme() {
    auto idx = last_idx_;
    Lexeme ret = get_lexeme();
    last_idx_ = idx;
    return ret;
}

const uint32_t Lexer::get_current_line() const {
    return current_line_; 
}

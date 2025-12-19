#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "token.h"

struct Lexeme {
    Token token {Undefined};
    std::string raw_val {""};
};

class Lexer {
public:
    Lexer(std::string file_path);
     
    Lexeme get_lexeme();
    Lexeme peek_next_lexeme();

    const uint32_t get_current_line() const;
private:
    char advance();
    char peek();
    char peek_next();
    uint64_t last_idx_ {0};
    uint32_t current_line_{0};

    std::string source_;
};

static std::unordered_map<std::string, Lexeme> lexeme_lookup_table {
    {"if", Lexeme(CtrlIf, "if")},
    {"else", Lexeme(CtrlElse, "else")},
    {"elseif", Lexeme(CtrlElseIf, "elseif")},
    {"while", Lexeme(CtrlWhile, "while")},
    {"for", Lexeme(CtrlFor, "for")},
    {"fn", Lexeme(FuncKword, "fn")},
    {"return", Lexeme(FuncRet, "return")},
    {"i32", Lexeme(TypeInt32, "i32")},
    {"u32", Lexeme(TypeUInt32, "u32")},
    {"f32", Lexeme(TypeFloat32, "f32")},
    {"str", Lexeme(TypeString, "str")},
    {"+", Lexeme(OpAdd, "+")},
    {"-", Lexeme(OpSub, "-")},
    {"/", Lexeme(OpDiv, "/")},
    {"*", Lexeme(OpMul, "*")},
    {"=", Lexeme(OpAssign, "=")},
    {"++", Lexeme(OpInc, "++")},
    {"--", Lexeme(OpDec, "--")},
    {"==", Lexeme(OpEqCheck, "==")},
    {"!=", Lexeme(OpNEqCheck, "!=")},
    {">", Lexeme(OpGreater, ">")},
    {"<", Lexeme(OpLess, "<")},
    {"=>", Lexeme(OpGreaterOrEq, ">=")},
    {"=<", Lexeme(OpLessOrEq, "<=")},
    {"(", Lexeme(ParanOpen, "(")},
    {")", Lexeme(ParanClose, ")")},
    {"{", Lexeme(CurBraceOpen, "{")},
    {"}", Lexeme(CurBraceClose, "}")},
    {":", Lexeme(Colon, ":")},
    {",", Lexeme(Comma, ",")},
};


#endif

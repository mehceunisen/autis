#ifndef TOKEN_H
#define TOKEN_H

#include <set>
#include <unordered_map>

enum Token {
    Undefined = -1,
    Identifier, // str ''x'
    CtrlIf, // if () {}
    CtrlElse, // else {}
    CtrlElseIf, // elseif () {}
    CtrlWhile, // while()
    CtrlFor, // for()
    FuncKword, // {fn_name}()
    FuncRet, // ret
    ParanOpen, // (
    ParanClose, // )
    CurBraceOpen, // {
    CurBraceClose, // }
    OpAdd, // +
    OpSub, // -
    OpDiv, // /
    OpMul, // *
    OpAssign, // =
    OpInc, // ++
    OpDec, // --
    OpEqCheck, // ==
    OpNEqCheck, // !=
    OpGreater, // >
    OpLess, // <
    OpGreaterOrEq, // =>
    OpLessOrEq, // =<
    TypeInt32, // i32
    TypeUInt32, // u32
    TypeFloat32, // float
    TypeString, // str
    IntLiteral, // 10
    UIntLiteral, // x >= 0
    FloatLiteral, // 10.2
    StringLiteral, // "test"
    Colon, // :
    Comma, // ,
    EndOfLine, // \n
    EndOfFile, // EOF
};

static std::unordered_map<Token, Token> type_to_literal {
    {TypeInt32, IntLiteral}, {TypeUInt32, UIntLiteral}, 
    {TypeFloat32, FloatLiteral}, {TypeString, StringLiteral},
};

static std::set<Token> operator_set {
    OpAdd,OpSub, OpDiv, OpMul,
        OpAssign, OpInc, OpDec, OpEqCheck,
        OpNEqCheck, OpGreater, OpLess, OpGreaterOrEq,
        OpLessOrEq
};

static std::set<Token> literal_set {
    IntLiteral, FloatLiteral, StringLiteral,
};

static std::set<Token> type_set {
    TypeInt32, TypeUInt32, TypeFloat32, TypeString,
};

static std::unordered_map<Token, int> binop_precedence {
    {OpLess, 10}, {OpGreater, 10}, {OpAdd, 20},
    {OpSub, 20}, {OpMul, 40}, {OpDiv, 50},
    {ParanOpen, 60}
    
};

#endif 

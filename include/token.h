#ifndef TOKEN_H
#define TOKEN_H

#include <set>

enum Token {
    Undefined = -1,
    Identifier, // str ''x'
    CtrlIf, // if () {}
    CtrlElse, // else {}
    CtrlElseIf, // elseif () {}
    CtrlWhile, // while()
    CtrlFor, // for()
    FuncDef, // fn
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
    TypeInt32, // int
    TypeUInt32, // u32
    TypeFloat32, // float
    TypeString, // str
    IntLiteral, // 10
    FloatLiteral, // 10.2
    StringLiteral, // "test"
    Colon, // :
    Comma, // ,
    EndOfLine, // \n
    EndOfFile, // EOF
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

#endif 

#ifndef TOKEN_H
#define TOKEN_H

#include <string.h>
#include <stdlib.h>

class Token 
{
public:
    enum Kind
    {
        TOKEN_HALT = 0,
        TOKEN_LOAD,
        TOKEN_STORE,
        TOKEN_READ,
        TOKEN_WRITE,
        TOKEN_COPY,
        TOKEN_PUSH,
        TOKEN_POP,
        TOKEN_ADD,
        TOKEN_SUB,
        TOKEN_MUL,
        TOKEN_DIV,
        TOKEN_MOD,
        TOKEN_AND,
        TOKEN_OR,
        TOKEN_NOT,
        TOKEN_JUMP,
        TOKEN_JZ,
        TOKEN_JN,
        TOKEN_CALL,
        TOKEN_RET,
        TOKEN_REG,
        TOKEN_NUM,
        TOKEN_WORD,
        TOKEN_END,
        TOKEN_SYMBOL,
        TOKEN_TWO_POINTS,
        TOKEN_EOF,
    };

    Kind t_kind;
    char* t_value;

    Token(const Token& other);
    Token();
    Token(Kind kind, const char* value);
    ~Token();
    Token& operator = (const Token& other);
};

#endif
#include "token.h"

Token::Token(const Token& other)
{
    this->t_kind = other.t_kind;
    this->t_value = (char*)malloc(sizeof(char) * strlen(other.t_value));
    strcpy(this->t_value, other.t_value);
}

Token::Token()
{
    this->t_kind = TOKEN_EOF;
    this->t_value = nullptr;
}

Token::Token(Kind kind, const char* value)
{
    this->t_kind = kind;
    this->t_value = (char*)malloc(sizeof(char) * strlen(value));
    strcpy(this->t_value, value);
}

Token::~Token()
{
    delete this->t_value;
}

Token& Token::operator = (const Token& other)
{
    this->t_kind = other.t_kind;
    this->t_value = (char*)malloc(sizeof(char) * strlen(other.t_value));
    strcpy(this->t_value, other.t_value);
    return *this;
}

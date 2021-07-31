#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <fstream>
#include <istream>
#include <iostream>

class Lexer
{
public:
    enum status 
    {
        LEXER_ERROR,
        LEXER_SUCCESS,
        LEXER_EOF,
    };

    char l_curr_char;
    int  l_head;
    int  l_line;
    int  l_line_pos;

    std::ifstream l_src;

    Token l_current_token;

    Token readToken();
    int skipComments();
    int skipSpaces();
    int skipBreakLines();
    status advance();
    status readNextToken();
    status collectToken();
    status collectNumber();
    status collectSymbols();
    Lexer(const char* filename);
    ~Lexer();
};

#endif

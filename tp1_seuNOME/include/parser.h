#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "lexer.h"

#include <vector>
#include <stdlib.h>

struct ASTNode
{
    enum ASTKind
    {
        AST_ROOT = 0,
        AST_HALT,
        AST_LOAD,
        AST_STORE,
        AST_READ,
        AST_WRITE,
        AST_COPY,
        AST_PUSH,
        AST_POP,
        AST_ADD,
        AST_SUB,
        AST_MUL,
        AST_DIV,
        AST_MOD,
        AST_AND,
        AST_OR,
        AST_NOT,
        AST_JUMP,
        AST_JZ,
        AST_JN,
        AST_CALL,
        AST_RET,
        AST_NUM,
        AST_WORD,
        AST_END,
        AST_LABEL,
        AST_SYMBOL,
        AST_REG,
    };

    ASTKind an_kind;
    char*   an_identifier;
    std::vector<ASTNode*> an_args;
   
    ASTNode* an_next;

    ASTNode(ASTKind kind, const char* identifier);
    ~ASTNode();
    void addArgument(ASTNode* arg);
};

class Parser
{
public:
    Lexer* p_lex;

    Parser(const char* filename);

    ASTNode* parse();
    ASTNode* parseInstruction();
    ASTNode* parseRegister();
    ASTNode* parseNum();
    ASTNode* parseSymbol();

    void print(ASTNode* root, int tabs = 0, bool break_line = false);
};


#endif
#include "parser.h"


ASTNode::ASTNode(ASTKind kind, const char* identifier)
{
    this->an_kind = kind;
    this->an_identifier = (char*)malloc(sizeof(char) * strlen(identifier));
    strcpy(this->an_identifier, identifier);
    this->an_next = nullptr;
}

void ASTNode::addArgument(ASTNode* arg)
{
    this->an_args.push_back(arg);
}

Parser::Parser(const char* filename)
{
    this->p_lex = new Lexer(filename);
}

ASTNode* Parser::parse()
{
    ASTNode* node = parseInstruction();
    ASTNode* root = node;

    while(node)
    {
        node->an_next = parseInstruction();
        node = node->an_next;
    }

    return root;
}

ASTNode* Parser::parseInstruction()
{
    Token token = this->p_lex->readToken();

    if(token.t_kind == Token::TOKEN_HALT)
    {
        return new ASTNode(ASTNode::AST_HALT, token.t_value);
    }

    if(token.t_kind == Token::TOKEN_LOAD)
    {
        ASTNode* load_op = new ASTNode(ASTNode::AST_LOAD, token.t_value);
        load_op->addArgument(this->parseRegister());
        load_op->addArgument(this->parseSymbol());
        return load_op;
    }

    if(token.t_kind == Token::TOKEN_STORE)
    {
        ASTNode* store_op = new ASTNode(ASTNode::AST_STORE, token.t_value);
        store_op->addArgument(this->parseRegister());
        store_op->addArgument(this->parseSymbol());
        return store_op;
    }

    if(token.t_kind == Token::TOKEN_READ)
    {
        ASTNode* read_op = new ASTNode(ASTNode::AST_READ, token.t_value);
        read_op->addArgument(this->parseRegister());
        return read_op;
    }

    if(token.t_kind == Token::TOKEN_WRITE)
    {
        ASTNode* write_op = new ASTNode(ASTNode::AST_WRITE, token.t_value);
        write_op->addArgument(this->parseRegister());
        return write_op;
    }

    if(token.t_kind == Token::TOKEN_COPY)
    {
        ASTNode* copy_op = new ASTNode(ASTNode::AST_COPY, token.t_value);
        copy_op->addArgument(this->parseRegister());
        copy_op->addArgument(this->parseRegister());
        return copy_op;
    }

    if(token.t_kind == Token::TOKEN_PUSH)
    {
        ASTNode* push_op = new ASTNode(ASTNode::AST_PUSH, token.t_value);
        push_op->addArgument(this->parseRegister());
        return push_op;
    }

    if(token.t_kind == Token::TOKEN_POP)
    {
        ASTNode* pop_op = new ASTNode(ASTNode::AST_POP, token.t_value);
        pop_op->addArgument(this->parseRegister());
        return pop_op;
    }

    if(token.t_kind == Token::TOKEN_ADD)
    {
        ASTNode* add_op = new ASTNode(ASTNode::AST_ADD, token.t_value);
        add_op->addArgument(this->parseRegister());
        add_op->addArgument(this->parseRegister());
        return add_op;
    }

    if(token.t_kind == Token::TOKEN_SUB)
    {
        ASTNode* sub_op = new ASTNode(ASTNode::AST_SUB, token.t_value);
        sub_op->addArgument(this->parseRegister());
        sub_op->addArgument(this->parseRegister());
        return sub_op;
    }

    if(token.t_kind == Token::TOKEN_MUL)
    {
        ASTNode* mul_op = new ASTNode(ASTNode::AST_MUL, token.t_value);
        mul_op->addArgument(this->parseRegister());
        mul_op->addArgument(this->parseRegister());
        return mul_op;
    }

    if(token.t_kind == Token::TOKEN_DIV)
    {
        ASTNode* div_op = new ASTNode(ASTNode::AST_DIV, token.t_value);
        div_op->addArgument(this->parseRegister());
        div_op->addArgument(this->parseRegister());
        return div_op;
    }

    if(token.t_kind == Token::TOKEN_MOD)
    {
        ASTNode* mod_op = new ASTNode(ASTNode::AST_MOD, token.t_value);
        mod_op->addArgument(this->parseRegister());
        mod_op->addArgument(this->parseRegister());
        return mod_op;
    }

    if(token.t_kind == Token::TOKEN_AND)
    {
        ASTNode* and_op = new ASTNode(ASTNode::AST_AND, token.t_value);
        and_op->addArgument(this->parseRegister());
        and_op->addArgument(this->parseRegister());
        return and_op;
    }

    if(token.t_kind == Token::TOKEN_OR)
    {
        ASTNode* or_op = new ASTNode(ASTNode::AST_OR, token.t_value);
        or_op->addArgument(this->parseRegister());
        or_op->addArgument(this->parseRegister());
        return or_op;
    }

    if(token.t_kind == Token::TOKEN_NOT)
    {
        ASTNode* not_op = new ASTNode(ASTNode::AST_NOT, token.t_value);
        not_op->addArgument(this->parseRegister());
        return not_op;
    }

    if(token.t_kind == Token::TOKEN_JUMP)
    {
        ASTNode* jump_op = new ASTNode(ASTNode::AST_JUMP, token.t_value);
        jump_op->addArgument(this->parseSymbol());
        return jump_op;
    }

    if(token.t_kind == Token::TOKEN_JZ)
    {
        ASTNode* jz_op = new ASTNode(ASTNode::AST_JZ, token.t_value);
        jz_op->addArgument(this->parseSymbol());
        return jz_op;
    }

    if(token.t_kind == Token::TOKEN_JN)
    {
        ASTNode* jn_op = new ASTNode(ASTNode::AST_JN, token.t_value);
        jn_op->addArgument(this->parseSymbol());
        return jn_op;
    }

    if(token.t_kind == Token::TOKEN_CALL)
    {
        ASTNode* call_op = new ASTNode(ASTNode::AST_CALL, token.t_value);
        call_op->addArgument(this->parseSymbol());
        return call_op;
    }

    if(token.t_kind == Token::TOKEN_WORD)
    {
        ASTNode* word_op = new ASTNode(ASTNode::AST_WORD, token.t_value);
        word_op->addArgument(this->parseSymbol());
        return word_op;
    }

    if(token.t_kind == Token::TOKEN_RET)
    {
        return new ASTNode(ASTNode::AST_RET, token.t_value);
    }

    if(token.t_kind == Token::TOKEN_WORD)
    {
        ASTNode* word_op = new ASTNode(ASTNode::AST_WORD, token.t_value);
        word_op->addArgument(this->parseSymbol());
        return word_op;
    }

    if(token.t_kind == Token::TOKEN_SYMBOL)
    {
        ASTNode* label_op = new ASTNode(ASTNode::AST_LABEL, token.t_value);
        Token two_points = this->p_lex->readToken();
        // assert(two_points.t_kind == Token::TOKEN_TWO_POINTS);
        return label_op;
    }


    if(token.t_kind == Token::TOKEN_END)
    {
        return new ASTNode(ASTNode::AST_END, token.t_value);
    }

    if(token.t_kind == Token::TOKEN_EOF)
    {
        return nullptr;
    }

    return nullptr;
}

ASTNode* Parser::parseRegister()
{
    Token reg_token = this->p_lex->readToken();
    ASTNode* reg_ast = new ASTNode(ASTNode::AST_REG, reg_token.t_value);
    return reg_ast;
}

ASTNode* Parser::parseNum()
{
    Token num_token = this->p_lex->readToken();
    // assert token is a num
    ASTNode* num_ast = new ASTNode(ASTNode::AST_NUM, num_token.t_value);
    return num_ast;
}

ASTNode* Parser::parseSymbol()
{
    Token sym_token = this->p_lex->readToken();

    ASTNode* sym_ast = nullptr;

    if(sym_token.t_kind == Token::TOKEN_NUM)
    {
        sym_ast = new ASTNode(ASTNode::AST_NUM, sym_token.t_value);
    }
    else
    {
        sym_ast = new ASTNode(ASTNode::AST_SYMBOL, sym_token.t_value);
    }

    return sym_ast;
}

void Parser::print(ASTNode* root, int tabs, bool break_line)
{
    if(!root) return;
    if(break_line) printf("\n");

    printf("%*c", tabs, ' ');
    if(break_line) printf("|___");

    std::cout << "<" << root->an_kind << ", " << root->an_identifier << "> ";

    for(ASTNode* arg : root->an_args)
    {
        print(arg, 0, false);
    }
    print(root->an_next, tabs+3, true);
}

#include "lexer.h"

Token Lexer::readToken()
{
    this->readNextToken();
    return this->l_current_token;
}

Lexer::status Lexer::readNextToken()
{
    while(
        this->skipComments() ||
        this->skipSpaces() ||
        this->skipBreakLines()
    ){}

    if(this->l_head == EOF)
    {
        this->l_current_token = Token(Token::TOKEN_EOF, "EOF");
        return LEXER_EOF;
    }

    if(this->l_curr_char == ':')
    {
        this->l_current_token = Token(Token::TOKEN_TWO_POINTS, ":");
        this->advance();
    }
    else if(this->l_curr_char == '-' || isdigit(this->l_curr_char)) this->collectNumber();
    else if(isalnum(this->l_curr_char)) this->collectToken();
    else
    {
        std::cout << "Syntax Error at character: '" << this->l_curr_char << "' in line " << this->l_line << " at position " << this->l_line_pos << "\n";
        exit(1);
    }

    return LEXER_SUCCESS;
}

Lexer::status Lexer::advance()
{
    if(!this->l_src.get(this->l_curr_char))
    {
        this->l_head = EOF;
        return LEXER_SUCCESS;
    }

    this->l_head++;
    this->l_line_pos++;

    return LEXER_SUCCESS;
}

int Lexer::skipComments()
{
    int skipped = 0;
    while(this->l_curr_char == ';')
    {
        while(this->l_curr_char != '\n')
        {
            this->advance();
            if(this->l_head == EOF)
            {
                return skipped;
            }
        }

        skipped++;
    }
    
    return skipped;
}

int Lexer::skipSpaces()
{
    int skipped = 0;
    
    while(this->l_curr_char == ' ')
    {
        this->advance();
        if(this->l_head == EOF)
        {
            return skipped;
        }

        skipped++;
    }
    
    return skipped;
}
int Lexer::skipBreakLines()
{
    int skipped = 0;

    while(this->l_curr_char == '\n')
    {
        this->l_line++;
        this->l_line_pos = 1;
        this->advance();
        if(this->l_head == EOF)
        {
            return skipped;
        }
        skipped++;
    }
    
    return skipped;
}

Lexer::status Lexer::collectToken()
{
    char* identifier = (char*)malloc(sizeof(char));

    identifier[0] = '\0';
    int p = 0;


    while(isalnum(this->l_curr_char) || isdigit(this->l_curr_char))
    {

        identifier = (char*)realloc(identifier, (p + 2)*sizeof(char));
        identifier[p] = this->l_curr_char;
        identifier[p+1] = '\0';
        p++;
        this->advance();

        if(this->l_head == EOF)
        {
            break;
        }
    }

    if(strlen(identifier) == 2 && identifier[0] == 'R' && isdigit(identifier[1]))
    {
        l_current_token = Token(Token::TOKEN_REG, identifier);
    }
    else if(strcmp(identifier, "HALT") == 0)    l_current_token = Token(Token::TOKEN_HALT, identifier);
    else if(strcmp(identifier, "READ") == 0)    l_current_token = Token(Token::TOKEN_READ, identifier);
    else if(strcmp(identifier, "LOAD") == 0)    l_current_token = Token(Token::TOKEN_LOAD, identifier);
    else if(strcmp(identifier, "STORE") == 0)   l_current_token = Token(Token::TOKEN_STORE, identifier);
    else if(strcmp(identifier, "READ") == 0)    l_current_token = Token(Token::TOKEN_READ, identifier);
    else if(strcmp(identifier, "WRITE") == 0)   l_current_token = Token(Token::TOKEN_WRITE, identifier);
    else if(strcmp(identifier, "COPY") == 0)    l_current_token = Token(Token::TOKEN_COPY, identifier);
    else if(strcmp(identifier, "PUSH") == 0)    l_current_token = Token(Token::TOKEN_PUSH, identifier);
    else if(strcmp(identifier, "POP") == 0)     l_current_token = Token(Token::TOKEN_POP, identifier);
    else if(strcmp(identifier, "ADD") == 0)     l_current_token = Token(Token::TOKEN_ADD, identifier);
    else if(strcmp(identifier, "SUB") == 0)     l_current_token = Token(Token::TOKEN_SUB, identifier);
    else if(strcmp(identifier, "MUL") == 0)     l_current_token = Token(Token::TOKEN_MUL, identifier);
    else if(strcmp(identifier, "DIV") == 0)     l_current_token = Token(Token::TOKEN_DIV, identifier);
    else if(strcmp(identifier, "MOD") == 0)     l_current_token = Token(Token::TOKEN_MOD, identifier);
    else if(strcmp(identifier, "AND") == 0)     l_current_token = Token(Token::TOKEN_AND, identifier);
    else if(strcmp(identifier, "OR") == 0)      l_current_token = Token(Token::TOKEN_OR, identifier);
    else if(strcmp(identifier, "NOT") == 0)     l_current_token = Token(Token::TOKEN_NOT, identifier);
    else if(strcmp(identifier, "JUMP") == 0)    l_current_token = Token(Token::TOKEN_JUMP, identifier);
    else if(strcmp(identifier, "JZ") == 0)      l_current_token = Token(Token::TOKEN_JZ, identifier);
    else if(strcmp(identifier, "JN") == 0)      l_current_token = Token(Token::TOKEN_JN, identifier);
    else if(strcmp(identifier, "CALL") == 0)    l_current_token = Token(Token::TOKEN_CALL, identifier);
    else if(strcmp(identifier, "RET") == 0)     l_current_token = Token(Token::TOKEN_RET, identifier);
    else if(strcmp(identifier, "WORD") == 0)    l_current_token = Token(Token::TOKEN_WORD, identifier);
    else if(strcmp(identifier, "END") == 0)     l_current_token = Token(Token::TOKEN_END, identifier);
    else                                        l_current_token = Token(Token::TOKEN_SYMBOL, identifier);

    delete identifier;
    
    return LEXER_SUCCESS;
}

Lexer::status Lexer::collectNumber()
{
    char* identifier = (char*)malloc(sizeof(char));

    identifier[0] = '\0';

    int p = 0;

    if(this->l_curr_char == '-')
    {
        identifier = (char*)realloc(identifier, (p + 2)*sizeof(char));
        identifier[p] = this->l_curr_char;
        identifier[p+1] = '\0';
        p++;
        this->advance();
    }

    while(isdigit(this->l_curr_char))
    {
        identifier = (char*)realloc(identifier, (p + 2)*sizeof(char));
        identifier[p] = this->l_curr_char;
        identifier[p+1] = '\0';
        p++;
        this->advance();

        if(this->l_head == EOF)
        {
            break;
        }
    }
    
    this->l_current_token = Token(Token::TOKEN_NUM, identifier);
    
    delete identifier;

    return LEXER_SUCCESS;
}

Lexer::status Lexer::collectSymbols()
{
    if(this->l_curr_char == ':')
    {
        this->l_current_token = Token(Token::TOKEN_TWO_POINTS, ":");
        if(this->advance() == LEXER_EOF)
        {
            return LEXER_EOF;
        }
    }
    return LEXER_SUCCESS;
}

Lexer::Lexer(const char* filename)
{
    this->l_head = 0;
    this->l_line = 1;
    this->l_line_pos = 1;
    this->l_src.open(filename, std::ifstream::in);
    this->l_curr_char = l_src.get();
    this->l_src.seekg (0, this->l_src.beg);
    this->l_src.get(this->l_curr_char);
}

Lexer::~Lexer()
{
    l_src.close();
}

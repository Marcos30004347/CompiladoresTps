#ifndef MONTADOR_H
#define MONTADOR_H

// TODO: structs, classes, e outras definições.

// TODO: currently, the parse can parse isntruction distributed across multiple lines
//       if this is not the desirable effect, move the lexer.skipBreakLines to the end
//       the parser parse instruction method before every return.

#include <map>
#include <vector>
#include <fstream>
#include <istream>
#include <ostream>
#include <string.h>
#include <string>
#include <assert.h>
#include <iostream>

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

    Token(const Token& other)
    {
        this->t_kind = other.t_kind;
        this->t_value = (char*)malloc(sizeof(char) * strlen(other.t_value));
        strcpy(this->t_value, other.t_value);
    }

    Token()
    {
        this->t_kind = TOKEN_EOF;
        this->t_value = nullptr;
    }

    Token(Kind kind, const char* value)
    {
        this->t_kind = kind;
        this->t_value = (char*)malloc(sizeof(char) * strlen(value));
        strcpy(this->t_value, value);
    }

    ~Token()
    {
        delete this->t_value;
    }

    Token& operator = (const Token& other)
    {
        this->t_kind = other.t_kind;
        this->t_value = (char*)malloc(sizeof(char) * strlen(other.t_value));
        strcpy(this->t_value, other.t_value);
        return *this;
    }
};

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

    Token readToken()
    {
        this->readNextToken();
        return this->l_current_token;
    }

    status readNextToken()
    {
        while(
            this->skipComments() ||
            this->skipSpaces() ||
            this->skipBreakLines()
        ){
        }


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
        else if(isdigit(this->l_curr_char)) this->collectNumber();
        else if(isalnum(this->l_curr_char)) this->collectToken();
        else
        {
            std::cout << "Syntax Error at character: '" << this->l_curr_char << "' in line " << this->l_line << " at position " << this->l_line_pos << "\n";
            // this->l_current_token = Token(Token::TOKEN_SYMBOL, ":");
            // this->advance();
        }

        return LEXER_SUCCESS;
    }

    status advance()
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

    int skipComments()
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

    int skipSpaces()
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
    int skipBreakLines()
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

    status collectToken()
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
        // std::cout << "HAHAHHAAH: " <<  identifier << std::endl;
     
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
    
    status collectNumber()
    {
        char* identifier = (char*)malloc(sizeof(char));
    
        identifier[0] = '\0';
    
        int p = 0;
    
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

    status collectSymbols()
    {
        if(this->l_curr_char == ':')
        {
            this->l_current_token = Token(Token::TOKEN_TWO_POINTS, ":");
            if(this->advance() == LEXER_EOF)
            {
                return LEXER_EOF;
            }
        }
    }

    Lexer(const char* filename)
    {
        this->l_head = 0;
        this->l_line = 1;
        this->l_line_pos = 1;
        this->l_src.open(filename, std::ifstream::in);
        this->l_curr_char = l_src.get();
        this->l_src.seekg (0, this->l_src.beg);
    }

    ~Lexer()
    {
        l_src.close();
    }
};

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

    ASTNode(ASTKind kind, const char* identifier)
    {
        this->an_kind = kind;
        this->an_identifier = (char*)malloc(sizeof(char) * strlen(identifier));
        strcpy(this->an_identifier, identifier);
        this->an_next = nullptr;
    }

    void addArgument(ASTNode* arg)
    {
        this->an_args.push_back(arg);
    }
};

class Parser
{
public:
    Lexer* p_lex;
    Parser(const char* filename)
    {
        this->p_lex = new Lexer(filename);
    }

    ASTNode* parse()
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

    ASTNode* parseInstruction()
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
            ASTNode* word_op = new ASTNode(ASTNode::AST_CALL, token.t_value);
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

    ASTNode* parseRegister()
    {
        Token reg_token = this->p_lex->readToken();
        ASTNode* reg_ast = new ASTNode(ASTNode::AST_REG, reg_token.t_value);
        return reg_ast;
    }

    ASTNode* parseNum()
    {
        Token num_token = this->p_lex->readToken();
        // assert token is a num
        ASTNode* num_ast = new ASTNode(ASTNode::AST_NUM, num_token.t_value);
        return num_ast;
    }

    ASTNode* parseSymbol()
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

    void print(ASTNode* root, int tabs = 0, bool break_line = false)
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
};

class Montador
{
public:
    // "Symbol -> address"
    std::map<std::string, int> symbol_table;
    
    // "Instruction -> code, size"
    std::map<std::string, std::tuple<int, int>> op_table;
    
    // "Pseudo Instruction -> size"
    std::map<std::string, std::tuple<int>> pop_table;
    
    size_t program_size;

    Parser* parser;
    ASTNode* root;
    
    Montador(const char* src)
    {
        this->parser = new Parser(src);
        this->root = this->parser->parse();    

        op_table["HALT"]    = std::tuple<int,int>(0, 1);
        op_table["LOAD"]    = std::tuple<int,int>(1, 3);
        op_table["STORE"]   = std::tuple<int,int>(2, 3);
        op_table["READ"]    = std::tuple<int,int>(3, 2);
        op_table["WRITE"]   = std::tuple<int,int>(4, 2);
        op_table["COPY"]    = std::tuple<int,int>(5, 3);
        op_table["PUSH"]    = std::tuple<int,int>(6, 2);
        op_table["POP"]     = std::tuple<int,int>(7, 2);
        op_table["ADD"]     = std::tuple<int,int>(8, 3);
        op_table["SUB"]     = std::tuple<int,int>(9, 3);
        op_table["MUL"]     = std::tuple<int,int>(10, 3);
        op_table["DIV"]     = std::tuple<int,int>(11, 3);
        op_table["MOD"]     = std::tuple<int,int>(12, 3);
        op_table["AND"]     = std::tuple<int,int>(13, 3);
        op_table["OR"]      = std::tuple<int,int>(14, 3);
        op_table["NOT"]     = std::tuple<int,int>(15, 2);
        op_table["JUMP"]    = std::tuple<int,int>(16, 2);
        op_table["JZ"]      = std::tuple<int,int>(17, 2);
        op_table["JN"]      = std::tuple<int,int>(18, 2);
        op_table["CALL"]    = std::tuple<int,int>(19, 2);
        op_table["RET"]     = std::tuple<int,int>(20, 1);
    
        pop_table["WORD"]   = std::tuple<int>(1);
        pop_table["END"]   = std::tuple<int>(0);
    }

    void phaseOne()
    {
        ASTNode* node = this->root;

        int location_counter = 0;

        while(node->an_kind != ASTNode::AST_END)
        {
            if(this->op_table.find(node->an_identifier) != this->op_table.end())
            {
                location_counter += std::get<1>(this->op_table[node->an_identifier]);
                for(ASTNode* arg : node->an_args)
                    if(arg->an_kind == ASTNode::AST_SYMBOL)
                        if(symbol_table.find(node->an_identifier) == symbol_table.end())
                            symbol_table[node->an_identifier] = -1;
            }
            else if(this->pop_table.find(node->an_identifier) != this->pop_table.end())
            {
                location_counter += std::get<0>(this->pop_table[node->an_identifier]);
            }
            else if(node->an_kind == ASTNode::AST_LABEL)
            {
                if(
                    symbol_table.find(node->an_identifier) == symbol_table.end() ||
                    symbol_table[node->an_identifier] == -1
                )
                {
                    symbol_table[node->an_identifier] = location_counter;
                } 
                else
                {
                    std::cout << node->an_identifier << " already defined at " << symbol_table[node->an_identifier] << "\n";
                }
            }
    
            node = node->an_next;
        }

        this->program_size = location_counter;
    }

    void phaseTwo()
    {
        int* program = new int[this->program_size]{0};
        
        ASTNode* node = this->root;
        
        int START = 0;
        
        while(node->an_kind == ASTNode::AST_LABEL)
        {
            START++;
            node = node->an_next->an_next;
        }
        
        node = this->root;

        int location_counter = 0;

        while(node->an_kind != ASTNode::AST_END)
        {
            if(this->op_table.find(node->an_identifier) != this->op_table.end())
            {
                program[location_counter] = std::get<0>(this->op_table[node->an_identifier]);

                int i = 1;

                for(ASTNode* arg : node->an_args)
                {
                    if(arg->an_kind == ASTNode::AST_SYMBOL)
                    {
                        program[location_counter + i] = symbol_table[arg->an_identifier] - location_counter - std::get<1>(this->op_table[node->an_identifier]);
                    }
                    else if(arg->an_kind == ASTNode::AST_REG)
                    {
                        program[location_counter + i] = atoi(&(arg->an_identifier[1]));
                    }
                    else if(arg->an_kind == ASTNode::AST_NUM)
                    {
                        program[location_counter + i] = atoi(arg->an_identifier);
                    }
                    i++;
                }

                location_counter += std::get<1>(this->op_table[node->an_identifier]);
            }
            else if(this->pop_table.find(node->an_identifier) != this->pop_table.end())
            {
                if(strcmp(node->an_identifier, "WORD") == 0)
                {
                    program[location_counter] = atoi(node->an_args[0]->an_identifier);
                }
    
                location_counter += std::get<0>(this->pop_table[node->an_identifier]);
            }

            node = node->an_next;
        }

        std::cout << "MV-EXE\n\n";
        int N = 100;
        int K = location_counter;
        
        std::cout << K << " ";
        std::cout << N << " ";
        std::cout << N+K+1000 << " ";
        std::cout << N+START << "\n\n";

        for(int i=0; i<(int)this->program_size; i++)
        {
            std::cout << program[i] << " ";
        }
        std::cout << "\n";
    }
};


#endif

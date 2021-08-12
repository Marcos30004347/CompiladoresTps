#include "montador.h"

Montador::Montador(const char* src)
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
    pop_table["END"]    = std::tuple<int>(0);

    // this->symbol_table = std::map<std::string, adrr>(0);
}

void Montador::phaseOne()
{
    ASTNode* node = this->root;

    int location_counter = 0;

    while(node->an_kind != ASTNode::AST_END)
    {
        if(this->op_table.find(node->an_identifier) != this->op_table.end())
        {
            location_counter += std::get<1>(this->op_table[node->an_identifier]);

            for(ASTNode* arg : node->an_args)
            {
                if(arg->an_kind == ASTNode::AST_SYMBOL)
                {
                    if(symbol_table.find(node->an_identifier) == symbol_table.end())
                    {
                        symbol_table.insert({ arg->an_identifier, {-1} });
                    }
                }
            }
        }
        else if(this->pop_table.find(node->an_identifier) != this->pop_table.end())
        {
            location_counter += std::get<0>(this->pop_table[node->an_identifier]);
        }
        else if(node->an_kind == ASTNode::AST_LABEL)
        {
            if(
                symbol_table.find(node->an_identifier) == symbol_table.end() ||
                symbol_table[node->an_identifier].value == -1
            )
            {
                symbol_table[node->an_identifier].value = location_counter;
            } 
        }

        node = node->an_next;
    }

    this->program_size = location_counter;
}

void Montador::phaseTwo()
{
    std::string* program = new std::string[this->program_size]{"\0"};

    ASTNode* node = this->root;
    
    int START = 0;
    
    while(node->an_kind == ASTNode::AST_LABEL)
    {
        node = node->an_next;
        // Add to start the size of the instruction defined in the label
        if(
            node->an_kind == ASTNode::AST_WORD ||
            node->an_kind == ASTNode::AST_END
        )
        {
            START += std::get<0>(this->pop_table[node->an_identifier]);
        }
        else
        {
            START += std::get<1>(this->op_table[node->an_identifier]);
        }
        node = node->an_next;
    }

    node = this->root;

    int location_counter = 0;

    while(node->an_kind != ASTNode::AST_END)
    {
        if(this->op_table.find(node->an_identifier) != this->op_table.end())
        {
            program[location_counter] = std::to_string(std::get<0>(this->op_table[node->an_identifier]));

            int i = 1;

            for(ASTNode* arg : node->an_args)
            {
                if(arg->an_kind == ASTNode::AST_SYMBOL)
                {
                    if(symbol_table[arg->an_identifier].value != -1)
                    {
                        program[location_counter + i] = std::to_string(symbol_table[arg->an_identifier].value - location_counter - std::get<1>(this->op_table[node->an_identifier]));
                    }
                    else
                    {
                        program[location_counter + i] = arg->an_identifier;
                    }
                }
                else if(arg->an_kind == ASTNode::AST_REG)
                {
                    program[location_counter + i] = arg->an_identifier[1];
                }
                else if(arg->an_kind == ASTNode::AST_NUM)
                {
                    program[location_counter + i] = arg->an_identifier;
                }
                i++;
            }

            location_counter += std::get<1>(this->op_table[node->an_identifier]);
        }
        else if(this->pop_table.find(node->an_identifier) != this->pop_table.end())
        {
            if(strcmp(node->an_identifier, "WORD") == 0)
            {
                program[location_counter] = node->an_args[0]->an_identifier;
            }

            location_counter += std::get<0>(this->pop_table[node->an_identifier]);
        }

        node = node->an_next;
    }

    int count = 0;
    for(std::map<std::string, adrr>::iterator it = this->symbol_table.begin(); it != this->symbol_table.end(); it++)
    {
        if(it->second.value != -1)
        {
            count++;
        }
    }

    std::cout << count << "\n";

    for(std::map<std::string, adrr>::iterator it = this->symbol_table.begin(); it != this->symbol_table.end(); it++)
    {
        if(it->second.value != -1)
        {
            std::cout << it->first << ":" << it->second.value << "\n";
        }
    }

    std::cout << "\n";

    // int N = 100;
    // int K = location_counter;
    
    // std::cout << K << " ";
    // std::cout << N << " ";
    // std::cout << N+K+1000 << " ";
    // std::cout << N+START << "\n\n";

    for(int i=0; i<(int)this->program_size; i++)
    {
        std::cout << program[i] << " ";
    }
    std::cout << "\n";
}

Montador::~Montador()
{
    delete this->parser;
    delete this->root;
}
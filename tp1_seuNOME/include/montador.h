#ifndef MONTADOR_H
#define MONTADOR_H

#include <map>
#include <vector>

#include "parser.h"

struct adrr {
    int value = -1;
};
 

class Montador
{
public:
    // "Symbol -> address"
    std::map<std::string, adrr> symbol_table;
    
    // "Instruction -> code, size"
    std::map<std::string, std::tuple<int, int>> op_table;
    
    // "Pseudo Instruction -> size"
    std::map<std::string, std::tuple<int>> pop_table;
    
    size_t program_size;

    Parser* parser;
    ASTNode* root;

    Montador(const char* src);
    ~Montador();

    void phaseOne();
    void phaseTwo();
};


#endif

#include <iostream>

#include "montador.h"

int main(int argc, char *argv[]) {
    // Parser parser(argv[1]);
    // ASTNode* root = parser.parse();

    // std::cout << "AST TREE:" << std::endl;
    // parser.print(root);
    // std::cout << "\n";

    Montador assembler(argv[1]);

    assembler.phaseOne();
    assembler.phaseTwo();

    return 0;
}

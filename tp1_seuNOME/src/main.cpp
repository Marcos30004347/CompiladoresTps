#include <iostream>

#include "montador.h"

int main(int argc, char *argv[]) {
    // Parser parser(argv[1]);
    // ASTNode* root = parser.parse();

    // std::cout << std::endl << std::endl << "AST TREE:" << std::endl;
    // parser.print(root);
    // std::cout << "\n";

    Montador assembler(argv[1]);

    assembler.phaseOne();
    assembler.phaseTwo();

    std::cout << std::endl;

    return 0;
}

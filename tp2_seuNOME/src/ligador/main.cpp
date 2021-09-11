#include <iostream>

#include "ligador.hpp"

int main(int argc, char *argv[]) {

    Linker *linker = new Linker();

    for (int i=1; i<argc; i++) {
        string fileName = argv[i];
        linker->readFile(fileName, i-1);
    }

    linker->run();

    return 0;
}

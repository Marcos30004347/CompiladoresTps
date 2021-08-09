#ifndef LINKER_H
#define LINKER_H

#include <bits/stdc++.h>

using namespace std;

struct File {
    int start;
    int end;
    int size;
    File(int start, int end) {
        this->start = start;
        this->end = end;
        this->size = end - start + 1;
    }
};

class Linker {
private:
    vector<File> files;
    vector<string> translation;
    map<string, pair<int, int>> labelTable;

public:
    void readFile(string filename);
    void addLabelToTable(string label, int relativePos, int fileIdx);
    void addToTranslation(string value);
    int getNewPosition(int currFile, string label);
    void run();
};


#endif

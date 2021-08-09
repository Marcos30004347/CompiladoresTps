#include "ligador.hpp"
#include "utils.hpp"

using namespace std;

Linker::Linker() {}
Linker::~Linker() {}

void Linker::readFile(string filename, int fileIdx) {
    fstream file;
    file.open(filename, ios::in);
    if (!file.is_open()) {
        error("could not open file");
    }

    string line;
    getline(file, line);
    int numOfLab = stoi(line);

    for (int i=0; i<numOfLab; i++) {
        getline(file, line);
        vector<string> info = splitString(line, ":");
        this->labelTable[info[0]] = make_pair(stoi(info[1]), fileIdx);
    }

    //blank line
    getline(file, line);

    getline(file, line);
    stringstream ss(line);
    string word;

    int start = this->translation.size();

    while (getline(ss, word, ' ')) {
        if (word == " " || word.length() == 0) {
            continue;
        }

        this->translation.push_back(word);
    }

    int end = this->translation.size() - 1;
    
    this->files.push_back(new File(start, end));

}

void Linker::printTable() {
    for (string i : this->translation) {
        cout << i << ": " << this->labelTable[i].first << " | " << this->labelTable[i].second << endl;
    }
}

void Linker::run() {
    this->printTable();
    debugA(this->translation, this->translation.size());
}
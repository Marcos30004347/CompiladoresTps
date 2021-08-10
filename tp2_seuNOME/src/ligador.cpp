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

int Linker::getNewPosition(int currFile, string label, int pc) {
    int relPos = this->labelTable[label].first;
    int fileIdx = this->labelTable[label].second;
    int fileStart = this->files[fileIdx]->start;
    
    int absPos = fileStart + relPos;

    int pos = absPos - pc;
    if (pos < 0) {
        pos++;
    } else {
        pos--;
    }

    return pos;
}

int Linker::getCurrFileIdx(int currPos) {
    for (long unsigned int i=0; i<this->files.size(); i++) {
        if (currPos >= this->files[i]->start && currPos <= this->files[i]->end) {
            return i;
        }
    }
    return -1;
}

void Linker::printTable() {
    for (string i : this->translation) {
        cout << i << ": " << this->labelTable[i].first << " | " << this->labelTable[i].second << endl;
    }
}

void Linker::printTranslation() {
    for (File *file : this->files) {
        for (int i = file->start; i<=file->end; i++) {
            cout << this->translation[i] << " ";
        }
        cout << " | ";
    }
    cout << endl;
}

void Linker::run() {

    // cout << "start file one: " << this->files[1]->start << endl;
    // this->printTable();
    // this->printTranslation();

    int N = 100;
    int K = this->translation.size();
    int START = this->files[labelTable["main"].second]->start + labelTable["main"].first; 

    cout << "MV-EXE" << endl;
    cout << K << " ";
    cout << N << " ";
    cout << N+K+1000 << " ";
    cout << N+START << "\n\n";

    for (long unsigned int i=0; i<this->translation.size(); i++) {
        string str = this->translation[i];
        if (isalpha(str[0])) {
            int currFile = this->getCurrFileIdx(i);
            int newPos = getNewPosition(currFile, str, i);
            cout << newPos << " ";
            continue;
        }
        cout << str << " ";
    }
    cout << endl;

    
}
#ifndef UTILS_H
#define UTILS_H

#define error(msg) { cout << "error: " << msg << endl; exit(EXIT_FAILURE); }
#define rep(i, a, b) for(int i = int(a); i < int(b); i++)
#define debugA(x, l) { rep(i,0,l) { cout << x[i] << " "; } printf("\n"); }

#include <bits/stdc++.h>

vector<string> splitString(string str, string delimiter) {
    vector<string> splitted;
    size_t pos = 0;
    std::string token;
    
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        splitted.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    splitted.push_back(str);

    return splitted;
}

#endif
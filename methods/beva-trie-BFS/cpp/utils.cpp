#include <iostream>
#include <sstream>
#include <string>
#include "../header/utils.h"
#include "../header/Beva.h"

using namespace std;

namespace beva_trie_bfs {

    void utils::split(string value, char delimiter, vector<string> &tokens) {
        stringstream check1(value);
        string intermediate;

        while (getline(check1, intermediate, delimiter)) {
            tokens.push_back(intermediate);
        }
    }

    void utils::printVector(int *vector, int size) {
        for (int i = 0; i < size; ++i) {
            cout << vector[i] << " ";
        }
        cout << "\n";
    }

    void utils::printTransition(int *from, int sizeFrom, int *to, int sizeTo, string &bitmap) {
        cout << "f([ ";
        for (int i = 0; i < sizeFrom; ++i) {
            cout << from[i] << " ";
        }
        cout << "], " + bitmap + ") = [ ";
        for (int i = 0; i < sizeTo; ++i) {
            cout << to[i] << " ";
        }
        cout << "]\n";
    }

}

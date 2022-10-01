#ifndef FUZZY_SUFFIX_ARRAY_UTILS_H
#define FUZZY_SUFFIX_ARRAY_UTILS_H

#include <vector>
#include <bitset>
#include <iostream>

using namespace std;

namespace fuzzy_suffix_array {

    class utils {
    public:
        static void split(string, char, vector<string> &);

        static inline int min(int a, int b, int c) {
            return std::min(std::min(a, b), c);
        };
    };

}

#endif //FUZZY_SUFFIX_ARRAY_UTILS_H

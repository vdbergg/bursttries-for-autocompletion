#include <iostream>
#include <sstream>
#include <string>
#include "../header/utils.h"

using namespace std;

namespace fuzzy_suffix_array {

    void utils::split(string value, char delimiter, vector<string> &tokens) {
        stringstream check1(value);
        string intermediate;

        while (getline(check1, intermediate, delimiter)) {
            tokens.push_back(intermediate);
        }
    }

}

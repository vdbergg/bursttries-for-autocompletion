#include <iostream>
#include <sstream>
#include <string>
#include "../header/utils.h"
#include "../header/Beva.h"

using namespace std;

namespace beva_bursttrie_dynamic {

    void utils::split(string value, char delimiter, vector<string> &tokens) {
        stringstream check1(value);
        string intermediate;

        while (getline(check1, intermediate, delimiter)) {
            tokens.push_back(intermediate);
        }
    }
}

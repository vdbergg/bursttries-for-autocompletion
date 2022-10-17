/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


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

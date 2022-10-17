/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include <iostream>
#include <unordered_map>

#include "Experiment.h"
#include "StaticString.h"

using namespace std;

namespace fuzzy_suffix_array {

    extern Experiment *experiment;
    extern unordered_map<string, string> config;
    extern vector<string> records;

}

#ifndef FUZZY_SUFFIX_ARRAY_SUFFIXARRAY_H
#define FUZZY_SUFFIX_ARRAY_SUFFIXARRAY_H

#include "StaticString.h"
#include "GlobalVariables.h"
#include "utils.h"

namespace fuzzy_suffix_array {

    class SuffixArray {
    public:
        SuffixArray(int editDistanceThreshold);

        ~SuffixArray();

        void build();

        void approximateSearch(const string &prefix, unordered_map<int, int> &resultsMap);

        vector<string> exactSearch(const string &prefix);

        pair<int, int> search(const string &prefix);

        vector<string> fetching(int beginRangeResult, int endRangeResult);

        vector<pair<unsigned, unsigned short>> suffixes;
        int editDistanceThreshold;
    };

}

#endif //FUZZY_SUFFIX_ARRAY_SUFFIXARRAY_H

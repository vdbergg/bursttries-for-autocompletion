#ifndef FUZZY_SUFFIX_ARRAY_FRAMEWORK_H
#define FUZZY_SUFFIX_ARRAY_FRAMEWORK_H

#include <vector>
#include <string>
#include "Experiment.h"
#include "GlobalVariables.h"
#include "SuffixArray.h"

using namespace std;

namespace fuzzy_suffix_array {

    class Framework {
    public:
        SuffixArray *suffixArray;
        vector<string> queries;
        int editDistanceThreshold;
        int dataset;

        Framework();

        void readData(string &, vector<StaticString> &);

        void readData(string &, vector<string> &, bool insertEndOfWord = false);

        void index();

        vector<string> processQuery(string &query, int queryId);

        vector<string> processFullQuery(string &query);

        ~Framework();

        static vector<string> output(const unordered_map<int, int> &map);
    };

}

#endif //FUZZY_SUFFIX_ARRAY_FRAMEWORK_H

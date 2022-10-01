#ifndef BEVA_TRIE_DFS_FRAMEWORK_H
#define BEVA_TRIE_DFS_FRAMEWORK_H

#include <vector>
#include <string>
#include "ActiveNode.h"
#include "Trie.h"
#include "Beva.h"
#include "Experiment.h"
#include "GlobalVariables.h"

using namespace std;

namespace beva_trie_dfs {

    class Framework {
    public:
        Trie *trie;
        vector<string> queries;
        vector<string> relevantQueries;
        int editDistanceThreshold;
        int dataset;

        Beva *beva;

        Framework();

        void readData(string &, vector<StaticString> &);

        void readData(string &, vector<string> &);

        void index();

        void process(string, int, int, vector<ActiveNode> &oldActiveNodes, vector<ActiveNode> &currentActiveNodes,
                     unsigned (&bitmaps)[CHAR_SIZE]);

        vector<char *> processFullQuery(string &query, int queryPosition = -1);

        vector<char *> processQuery(string &query, int queryId);

        vector<char *> output(vector<ActiveNode> &currentActiveNodes);

        void writeExperiments();

        ~Framework();
    };

}

#endif //BEVA_TRIE_DFS_FRAMEWORK_H

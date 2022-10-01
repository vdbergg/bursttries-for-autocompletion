#ifndef BEVA_BURSTTRIE_BFS_FRAMEWORK_H
#define BEVA_BURSTTRIE_BFS_FRAMEWORK_H

#include <vector>
#include <string>
#include "ActiveNode.h"
#include "Trie.h"
#include "Beva.h"
#include "Experiment.h"

using namespace std;

namespace beva_bursttrie_bfs {

    class Framework {
    public:
        Trie *trie;
        vector<string> queries;
        vector<string> relevantQueries;
        int editDistanceThreshold;
        int dataset;
        Experiment *experiment;
        unordered_map<string, string> config;

        Beva *beva;

        Framework(unordered_map<string, string>);

        void readData(string &, vector<string> &);

        void readData(string &, vector<StaticString> &);

        void index();

        void process(string, int, int, vector<ActiveNode> &oldActiveNodes,
                     vector<ActiveNode> &currentActiveNodes, unsigned (&bitmaps)[CHAR_SIZE]);

        vector<char *> processFullQuery(string &query, int queryId = -1);

        vector<char *> processQuery(string &query, int queryId);

        vector<char *> output(vector<ActiveNode> &currentActiveNodes);

        ~Framework();

        void writeExperiments();
    };

}

#endif //BEVA_BURSTTRIE_BFS_FRAMEWORK_H

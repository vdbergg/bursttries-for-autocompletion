#ifndef BEVA_TRIE_DFS_EXPERIMENT_H
#define BEVA_TRIE_DFS_EXPERIMENT_H

#include "Directives.h"

#ifndef BEVA_IS_MAC_H
#include "sys/sysinfo.h"
#else
#include "sys/types.h"
#endif

#include <vector>
#include <chrono>

using namespace std;

namespace beva_trie_dfs {

    class Experiment {
    public:
        Experiment(unordered_map<string, string>, int);

        ~Experiment();

        unordered_map<string, string> config;
        int editDistanceThreshold;
        long numberOfNodes;
        vector<unsigned long> numberOfActiveNodes;
        vector<unsigned long> numberOfIterationInChildren;
        vector<long> processingTimes;
        vector<long> fetchingTimes;
        vector<long> resultsSize;
        unsigned long simpleProcessingTimes;
        unsigned long simpleFetchingTimes;
        unsigned long simpleResultsSize;
        vector<long> currentQueryProcessingTime;
        vector<long> currentQueryFetchingTime;
        vector<long> currentResultsSize;
        vector<long> activeNodesSizes;
        unsigned long simpleActiveNodesSizes;
        vector<long> currentActiveNodesSize;
        unordered_map<int, int> branchSize;

#ifndef BEVA_IS_MAC_H
        chrono::time_point<std::chrono::system_clock> startIndexingTime;
        chrono::time_point<std::chrono::system_clock> finishIndexingTime;

        chrono::time_point<std::chrono::system_clock> startQueryProcessingTime;
        chrono::time_point<std::chrono::system_clock> finishQueryProcessingTime;

        chrono::time_point<std::chrono::system_clock> startQueryFetchingTime;
        chrono::time_point<std::chrono::system_clock> finishQueryFetchingTime;
#else
        chrono::high_resolution_clock::time_point startIndexingTime;
        chrono::high_resolution_clock::time_point finishIndexingTime;

        chrono::high_resolution_clock::time_point startQueryProcessingTime;
        chrono::high_resolution_clock::time_point finishQueryProcessingTime;

        chrono::high_resolution_clock::time_point startQueryFetchingTime;
        chrono::high_resolution_clock::time_point finishQueryFetchingTime;
#endif

        void initIndexingTime();

        void endIndexingTime();

        void initQueryProcessingTime();

        void endQueryProcessingTime(long, int);

        void endSimpleQueryProcessingTime(long);

        void initQueryFetchingTime();

        void endQueryFetchingTime(int, unsigned long);

        void endSimpleQueryFetchingTime(unsigned long);

        void compileQueryProcessingTimes(int);

        void compileSimpleQueryProcessingTimes(string &, bool relevantReturned = false);

        void proportionOfBranchingSize(int size);

        void incrementNumberOfNodes();

        void incrementNumberOfActiveNodes(unsigned);

        void compileNumberOfActiveNodes();

        void incrementNumberOfIterationInChildren(unsigned);

        void compileNumberOfIterationInChildren();

        void compileNumberOfNodes();

        void compileProportionOfBranchingSizeInBEVA2Level();

        void saveQueryProcessingTime(string &, int);

        void getMemoryUsedInIndexing();

        void getMemoryUsedInProcessing();

        void writeFile(const string &, const string &, bool writeInTheEnd = false);
    };

}

#endif //BEVA_TRIE_DFS_EXPERIMENT_H

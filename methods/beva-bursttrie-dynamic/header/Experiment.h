#ifndef BEVA_BURSTTRIE_DYNAMIC_EXPERIMENT_H
#define BEVA_BURSTTRIE_DYNAMIC_EXPERIMENT_H
#include <vector>
#include <chrono>
#include <string>
#include <unordered_map>
#include <sstream>

#include "../header/Directives.h"

#ifndef IS_MAC_H
#include "sys/sysinfo.h"
#else
#include "sys/types.h"
#endif

using namespace std;

namespace beva_bursttrie_dynamic {

    class Experiment {
    public:
        Experiment(unordered_map<string, string>, int);

        ~Experiment();

        unordered_map<string, string> config;
        int editDistanceThreshold;
        long numberOfNodes;
        vector<unsigned long> numberOfActiveNodes;
        vector<unsigned long> numberOfWordActiveNodes;
        vector<unsigned long> numberOfIterationInChildren;
        vector<unsigned long> numberOfIterationInChildren2Level;
        vector<long> processingTimes;
        vector<long> fetchingTimes;
        vector<long> resultsSize;
        unsigned long simpleProcessingTimes;
        unsigned long simpleFetchingTimes;
        unsigned long simpleResultsSize;
        vector<long> currentResultsSize;
        vector<long> currentQueryProcessingTime;
        vector<long> currentQueryFetchingTime;
        vector<long> activeNodesSizes;
        unsigned long simpleActiveNodesSizes;
        vector<long> currentActiveNodesSize;
        unordered_map<int, int> branchSize;

#ifndef IS_MAC_H
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

        void proportionOfBranchingSizeInBEVA2Level(int);

        void incrementNumberOfNodes();

        void compileNumberOfNodes();

        void incrementNumberOfWordActiveNodes(unsigned querySize);

        void compileNumberOfWordActiveNodes();

        void incrementNumberOfActiveNodes(unsigned querySize);

        void compileNumberOfActiveNodes();

        void incrementNumberOfIterationInChildren(unsigned querySize);

        void compileNumberOfIterationInChildren();

        void incrementNumberOfIterationInChildren2Level(unsigned querySize);

        void compileNumberOfIterationInChildren2Level();

        void compileProportionOfBranchingSizeInBEVA2Level();

        void saveQueryProcessingTime(string &, int);

        void getMemoryUsedInIndexing();

        void getMemoryUsedInProcessing();

        void writeFile(const string &, const string &, bool writeInTheEnd = false);
    };

}

#endif //BEVA_BURSTTRIE_DYNAMIC_EXPERIMENT_H

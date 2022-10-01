#ifndef FUZZY_SUFFIX_ARRAY_EXPERIMENT_H
#define FUZZY_SUFFIX_ARRAY_EXPERIMENT_H

#include "../header/Directives.h"

#ifndef FUZZY_SUFFIX_ARRAY_IS_MAC_H
#include "sys/sysinfo.h"
#else
#include "sys/types.h"
#endif

#include <vector>
#include <chrono>
#include <unordered_map>

using namespace std;

namespace fuzzy_suffix_array {

    class Experiment {
    public:
        Experiment(unordered_map<string, string> config, int);

        ~Experiment();

        unordered_map<string, string> config;
        int editDistanceThreshold;
        vector<long> processingTimes;
        vector<long> fetchingTimes;
        vector<long> resultsSize;
        unsigned long simpleProcessingTimes;
        unsigned long simpleFetchingTimes;
        unsigned long simpleResultsSize;
        vector<long> currentQueryProcessingTime;
        vector<long> currentQueryFetchingTime;
        vector<long> currentResultsSize;

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

        void endQueryProcessingTime(int);

        void endSimpleQueryProcessingTime();

        void initQueryFetchingTime();

        void endQueryFetchingTime(int, unsigned long);

        void endSimpleQueryFetchingTime(unsigned long);

        void compileQueryProcessingTimes(int);

        void compileSimpleQueryProcessingTimes(string &);

        void saveQueryProcessingTime(string &, int);

        void getMemoryUsedInIndexing();

        void getMemoryUsedInProcessing();

        void writeFile(const string &, const string &, bool writeInTheEnd = false);
    };

}

#endif //FUZZY_SUFFIX_ARRAY_EXPERIMENT_H

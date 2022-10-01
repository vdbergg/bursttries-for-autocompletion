#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <unistd.h>
#include <algorithm>

#include "../header/Experiment.h"
#include "../header/Directives.h"
#include "../header/utils.h"

using namespace std;

namespace fuzzy_suffix_array {

    void exec(const char *cmd, string &result) {
        char buffer[128];
        FILE *pipe = popen(cmd, "r");
        if (!pipe) throw std::runtime_error("popen() failed!");
        try {
            while (fgets(buffer, sizeof buffer, pipe) != NULL) {
                result += buffer;
            }
        } catch (...) {
            pclose(pipe);
            throw;
        }
        pclose(pipe);
    }

    Experiment::Experiment(unordered_map<string, string> config, int editDistanceThreshold) {
        this->config = std::move(config);
        this->editDistanceThreshold = editDistanceThreshold;
    }

    Experiment::~Experiment() {
    }

    void Experiment::writeFile(const string &name, const string &value, bool writeInTheEnd) {
        ofstream myfile;
        string buildIndexType = "bfs";

        string newName = config["experiments_basepath"] + name;
        newName += "_data_set_" + config["dataset"] + "_size_type_" + config["size_type"] + "_tau_" +
                   to_string(this->editDistanceThreshold) + "_index_type_" + buildIndexType + "_alg_" +
                   config["alg"] + ".txt";

        if (writeInTheEnd) {
            myfile.open(newName, std::ios::app);
        } else {
            myfile.open(newName);
        }

        if (myfile.is_open()) {
            myfile << value;
            myfile.close();
        } else {
            cout << "Unable to open file.\n";
        }
    }

    void setVector(int position, unsigned long value, vector<long> &v) {
        if (position < v.size()) {
            v[position] += value;
        } else {
            for (int i = v.size(); i <= position; i++) {
                if (i == position) {
                    v.push_back(value);
                } else {
                    v.push_back(0);
                }
            }
        }
    }

    void Experiment::initIndexingTime() {
        this->startIndexingTime = chrono::high_resolution_clock::now();
    }

    void Experiment::endIndexingTime() {
        this->finishIndexingTime = chrono::high_resolution_clock::now();

        long result = chrono::duration_cast<chrono::nanoseconds>(
                this->finishIndexingTime - this->startIndexingTime
        ).count();

        writeFile("indexing_time", "indexing_time\n" + to_string(result) + "\n");
    }

    void Experiment::initQueryFetchingTime() {
        this->startQueryFetchingTime = chrono::high_resolution_clock::now();
    }

    void Experiment::endQueryFetchingTime(int prefixQueryLength, unsigned long resultsSize_) {
        this->finishQueryFetchingTime = chrono::high_resolution_clock::now();

        long result = chrono::duration_cast<chrono::nanoseconds>(
                this->finishQueryFetchingTime - this->startQueryFetchingTime
        ).count();

        setVector(prefixQueryLength - 1, result, this->currentQueryFetchingTime);
        setVector(prefixQueryLength - 1, resultsSize_, this->currentResultsSize);
        setVector(prefixQueryLength - 1, result, this->fetchingTimes);
        setVector(prefixQueryLength - 1, resultsSize_, this->resultsSize);
    }

    void Experiment::endSimpleQueryFetchingTime(unsigned long resultsSize_) {
        this->finishQueryFetchingTime = chrono::high_resolution_clock::now();

        this->simpleFetchingTimes = chrono::duration_cast<chrono::nanoseconds>(
                this->finishQueryFetchingTime - this->startQueryFetchingTime
        ).count();

        this->simpleResultsSize = resultsSize_;
    }

    void Experiment::initQueryProcessingTime() {
        this->startQueryProcessingTime = chrono::high_resolution_clock::now();
    }

    void Experiment::endSimpleQueryProcessingTime() {
        this->finishQueryProcessingTime = chrono::high_resolution_clock::now();

        this->simpleProcessingTimes = chrono::duration_cast<chrono::nanoseconds>(
                this->finishQueryProcessingTime - this->startQueryProcessingTime
        ).count();
    }

    void Experiment::endQueryProcessingTime(int prefixQueryLength) {
        this->finishQueryProcessingTime = chrono::high_resolution_clock::now();

        long result = chrono::duration_cast<chrono::nanoseconds>(
                this->finishQueryProcessingTime - this->startQueryProcessingTime
        ).count();

        setVector(prefixQueryLength - 1, result, this->currentQueryProcessingTime);
        setVector(prefixQueryLength - 1, result, this->processingTimes);
    }

    void Experiment::saveQueryProcessingTime(string &query, int queryId) {
        string value = query + "\t" + to_string(queryId) + "\n";

        for (int j = 0; j < this->currentQueryProcessingTime.size(); j++) {
            value += to_string(j + 1) + "\t" + to_string(this->currentQueryProcessingTime[j]) + "\t" +
                     to_string(this->currentQueryFetchingTime[j]) + "\t" +
                     to_string(this->currentResultsSize[j]) + "\n";
        }
        this->currentResultsSize.clear();
        this->currentQueryProcessingTime.clear();
        this->currentQueryFetchingTime.clear();

        writeFile("all_time_values", value, true);
    }

    void Experiment::compileQueryProcessingTimes(int queryId) {
        string value = to_string(queryId) + "\n";
        value += "query_size\tquery_processing_time\tfetching_time\tresults_size\n";

        for (int i = 0; i < this->processingTimes.size(); i++) {
            long processingTime = this->processingTimes[i] / (queryId + 1);
            long fetchingTime = this->fetchingTimes[i] / (queryId + 1);
            float _resultsSize = this->resultsSize[i] / (float) (queryId + 1);
            stringstream streamResultSize;
            streamResultSize << std::fixed << std::setprecision(1) << _resultsSize;
            value += to_string(i + 1) + "\t" + to_string(processingTime) + "\t" + to_string(fetchingTime) +
                     "\t" + streamResultSize.str() + "\n";
        }

        writeFile("query_processing_time", value);
    }

    void Experiment::compileSimpleQueryProcessingTimes(string &query) {
        string value = query + "\t" + to_string(this->simpleProcessingTimes) + "\t" +
                       to_string(this->simpleFetchingTimes) + "\t" + to_string(this->simpleResultsSize) + "\n";

        writeFile("all_time_values", value, true);
    }

    void Experiment::getMemoryUsedInProcessing() {
        pid_t pid = getpid();
        string cmd = "/bin/ps -p " + to_string(pid) + " -o size";
        string output;
        exec(cmd.c_str(), output);

        vector<string> tokens;
        utils::split(output, '\n', tokens);

        float memoryUsed = stof(tokens[1]) / 1000;

        tokens.clear();
        tokens.shrink_to_fit();

        writeFile("memory_used_in_processing", to_string(memoryUsed) + "\n", true);
    }

    void Experiment::getMemoryUsedInIndexing() {
        pid_t pid = getpid();
        string cmd = "/bin/ps -p " + to_string(pid) + " -o size";
        string output;
        exec(cmd.c_str(), output);

        vector<string> tokens;
        utils::split(output, '\n', tokens);

        float memoryUsed = stof(tokens[1]) / 1000;

        string value = to_string(memoryUsed) + "\n";

        tokens.clear();
        tokens.shrink_to_fit();

        writeFile("memory_used_in_indexing", "memory_total_mb\tmemory_used_mb\n" + value);
    }

}

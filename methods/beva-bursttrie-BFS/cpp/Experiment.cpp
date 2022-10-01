#include <utility>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <algorithm>

#include "../header/Experiment.h"
#include "../header/Directives.h"
#include "../header/utils.h"

using namespace std;

namespace beva_bursttrie_bfs {

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
                   to_string(this->editDistanceThreshold) + "_index_level_threshold_" +
                   config["index_level_threshold"] +
                   "_max_second_level_size_" + config["max_second_level_size"] + "_index_type_" + buildIndexType +
                   "_alg_" + config["alg"] + ".txt";

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

    void Experiment::endQueryProcessingTime(long activeNodesSize, int prefixQueryLength) {
        this->finishQueryProcessingTime = chrono::high_resolution_clock::now();

        long result = chrono::duration_cast<chrono::nanoseconds>(
                this->finishQueryProcessingTime - this->startQueryProcessingTime
        ).count();

        setVector(prefixQueryLength - 1, result, this->currentQueryProcessingTime);
        setVector(prefixQueryLength - 1, activeNodesSize, this->currentActiveNodesSize);
        setVector(prefixQueryLength - 1, result, this->processingTimes);
        setVector(prefixQueryLength - 1, activeNodesSize, this->activeNodesSizes);
    }

    void Experiment::endSimpleQueryProcessingTime(long activeNodesSize) {
        this->finishQueryProcessingTime = chrono::high_resolution_clock::now();

        this->simpleProcessingTimes = chrono::duration_cast<chrono::nanoseconds>(
                this->finishQueryProcessingTime - this->startQueryProcessingTime
        ).count();

        this->simpleActiveNodesSizes = activeNodesSize;
    }

    void Experiment::saveQueryProcessingTime(string &query, int queryId) {
        string value = query + "\t" + to_string(queryId) + "\n";

        long accum = 0;

        for (int j = 0; j < this->currentQueryProcessingTime.size(); j++) {
            accum += this->currentQueryProcessingTime[j];

            value += to_string(j + 1) + "\t" + to_string(this->currentQueryProcessingTime[j]) +
                     "\t" + to_string(accum) + "\t" + to_string(this->currentQueryFetchingTime[j]) +
                     "\t" + to_string(this->currentResultsSize[j]) +
                     "\t" + to_string(this->currentActiveNodesSize[j]) + "\n";
        }
        this->currentResultsSize.clear();
        this->currentActiveNodesSize.clear();
        this->currentQueryProcessingTime.clear();
        this->currentQueryFetchingTime.clear();

        writeFile("all_time_values", value, true);
    }

    void Experiment::compileQueryProcessingTimes(int queryId) {
        string value = to_string(queryId) + "\n";
        value += "query_size\tquery_processing_time\taccumulated_query_processing_time\tfetching_time\tresults_size\t"
                 "\tactive_nodes_size\n";

        long accumGeneral = 0;

        for (int i = 0; i < this->processingTimes.size(); i++) {
            long processingTime = this->processingTimes[i] / (queryId + 1);
            long fetchingTime = this->fetchingTimes[i] / (queryId + 1);
            float resultsSize_ = this->resultsSize[i] / (float) (queryId + 1);
            stringstream streamResultSize;
            streamResultSize << std::fixed << std::setprecision(1) << resultsSize_;

            accumGeneral += processingTime;
            float activeNodesSize = this->activeNodesSizes[i] / (float) (queryId + 1);
            stringstream streamActiveNodesSizes;
            streamActiveNodesSizes << std::fixed << std::setprecision(1) << activeNodesSize;

            value += to_string(i + 1) + "\t" + to_string(processingTime) + "\t" + to_string(accumGeneral) + "\t" +
                     to_string(fetchingTime) + "\t" + streamResultSize.str() + "\t" + streamActiveNodesSizes.str() +
                     "\n";
        }

        writeFile("query_processing_time", value);
    }

    void Experiment::compileSimpleQueryProcessingTimes(string &query, bool relevantReturned) {
        string value = query + "\t" + to_string(this->simpleProcessingTimes) + "\t" +
                       to_string(this->simpleFetchingTimes) + "\t" + to_string(this->simpleResultsSize) + "\t" +
                       to_string(this->simpleActiveNodesSizes) + "\t" + to_string(int(relevantReturned)) + "\n";

        writeFile("all_time_values", value, true);
    }

    void Experiment::proportionOfBranchingSizeInBEVA2Level(int size) {
        if (this->branchSize.find(size) == this->branchSize.end()) {
            this->branchSize[size] = 1;
        } else {
            this->branchSize[size]++;
        }
    }

    void Experiment::compileProportionOfBranchingSizeInBEVA2Level() {
        string value = "branch_size\tnumber_of_branches\n";

        vector<pair<int, int>> elements(this->branchSize.begin(), this->branchSize.end());
        sort(elements.begin(), elements.end());

        for (const pair<int, int> &p : elements) {
            value += to_string(p.first) + " " + to_string(p.second) + "\n";
        }
        elements.shrink_to_fit();
        elements.clear();
        this->branchSize.clear();
        writeFile("proportion_branch_size", value);
    }

    void Experiment::incrementNumberOfNodes() {
        this->numberOfNodes++;
    }

    void Experiment::incrementNumberOfWordActiveNodes(unsigned querySize) {
        this->numberOfWordActiveNodes[querySize] += 1;
    }

    void Experiment::incrementNumberOfActiveNodes(unsigned querySize) {
        this->numberOfActiveNodes[querySize] += 1;
    }

    void Experiment::incrementNumberOfIterationInChildren(unsigned querySize) {
        this->numberOfIterationInChildren[querySize] += 1;
    }

    void Experiment::incrementNumberOfIterationInChildren2Level(unsigned querySize) {
        this->numberOfIterationInChildren2Level[querySize] += 1;
    }

    void Experiment::compileNumberOfWordActiveNodes() {
        string value = "number_of_word_active_nodes\n";
        for (int i = 0; i < this->numberOfWordActiveNodes.size(); i++) {
            value += to_string(i + 1) + "\t" + to_string(this->numberOfWordActiveNodes[i]) + "\n";
            this->numberOfWordActiveNodes[i] = 0;
        }
        writeFile("number_of_word_active_nodes", value);
    }

    void Experiment::compileNumberOfActiveNodes() {
        string value = "number_of_active_nodes\n";
        for (int i = 0; i < this->numberOfActiveNodes.size(); i++) {
            value += to_string(i + 1) + "\t" + to_string(this->numberOfActiveNodes[i]) + "\n";
            this->numberOfActiveNodes[i] = 0;
        }
        writeFile("number_of_active_nodes", value);
    }

    void Experiment::compileNumberOfIterationInChildren() {
        string value = "number_of_iteration_in_children\n";
        for (int i = 0; i < this->numberOfIterationInChildren.size(); i++) {
            value += to_string(i + 1) + "\t" + to_string(this->numberOfIterationInChildren[i]) + "\n";
            this->numberOfIterationInChildren[i] = 0;
        }
        writeFile("number_of_iteration_in_children", value);
    }

    void Experiment::compileNumberOfIterationInChildren2Level() {
        string value = "number_of_iteration_in_children_2_level\n";
        for (int i = 0; i < this->numberOfIterationInChildren2Level.size(); i++) {
            value += to_string(i + 1) + "\t" + to_string(this->numberOfIterationInChildren2Level[i]) + "\n";
            this->numberOfIterationInChildren2Level[i] = 0;
        }
        writeFile("number_of_iteration_in_children_2_level", value);
    }

    void Experiment::compileNumberOfNodes() {
        string value = "number_of_nodes\n";
        value += to_string(this->numberOfNodes) + "\n";
        writeFile("number_of_nodes", value);
    }

    void Experiment::getMemoryUsedInProcessing() {
        pid_t pid = getpid();
        string cmd = "/bin/ps -p " + to_string(pid) + " -o size";
        string output;
        exec(cmd.c_str(), output);

        vector<string> tokens;
        utils::split(output, '\n', tokens);

        float memoryUsed = stof(tokens[1]) / 1000;

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

        writeFile("memory_used_in_indexing", "memory_total_mb\tmemory_used_mb\n" + value);
    }

}

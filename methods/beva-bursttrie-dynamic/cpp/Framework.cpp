#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <sys/stat.h>
#include "../header/C.h"
#include "../header/Framework.h"
#include "../header/Directives.h"

using namespace std;

namespace beva_bursttrie_dynamic {

    Experiment *experiment;

    Framework::Framework() {
        this->trie = nullptr;
        this->editDistanceThreshold = stoi(config["edit_distance"]);
        this->dataset = stoi(config["dataset"]);
        experiment = new Experiment(config, editDistanceThreshold);

        index();
    }

    Framework::~Framework() {
        cout << "deleting framework" << endl;
        delete this->beva;
        delete this->trie;
        delete experiment;
    }

    unsigned long getFileSize(string filename) {
        FILE *fp = fopen(filename.c_str(), "r");

        struct stat buf;
        fstat(fileno(fp), &buf);
        fclose(fp);
        return buf.st_size;
    }

    void Framework::readData(string &filename, vector<StaticString> &recs) {
        cout << "reading dataset " << filename << endl;

        string str;
        ifstream input(filename, ios::in);

        unsigned long fileSize = getFileSize(filename);
        char *tmpPtr = (char *) malloc(sizeof(char) * fileSize);
        StaticString::setDataBaseMemory(tmpPtr, fileSize);
        while (getline(input, str)) {
            if (!str.empty()) recs.push_back(StaticString(str));
        }
    }

    void Framework::readData(string &filename, vector<string> &recs) {
        cout << "reading dataset " << filename << endl;

        string str;
        ifstream input(filename, ios::in);
        while (getline(input, str)) {
            if (!str.empty()) recs.push_back(str);
        }
    }

    void Framework::index() {
        cout << "indexing... \n";
        string sizeSufix = "";
        switch (stoi(config["size_type"])) {
            case 0:
                sizeSufix = "_20";
                break;
            case 1:
                sizeSufix = "_40";
                break;
            case 2:
                sizeSufix = "_60";
                break;
            case 3:
                sizeSufix = "_80";
                break;
            case 4:
                sizeSufix = "";
                break;
            default:
                sizeSufix = "";
        }

        auto start = chrono::high_resolution_clock::now();

#ifdef BEVA_IS_COLLECT_TIME_H
        experiment->initIndexingTime();
#endif

        string datasetFile = config["dataset_basepath"];
        string queryFile = config["query_basepath"];
        string relevantQueryFile = config["query_basepath"];

        int queriesSize = stoi(config["queries_size"]);
        string datasetSuffix = queriesSize == 10 ? "_10" : "";
        string tau = to_string(this->editDistanceThreshold);

        switch (this->dataset) {
            case C::AOL:
                datasetFile += "aol/aol" + sizeSufix + ".txt";
                queryFile += "aol/q17_" + tau + datasetSuffix + ".txt";
                break;
            case C::MEDLINE:
                datasetFile += "medline/medline" + sizeSufix + ".txt";
                queryFile += "medline/q13" + datasetSuffix + ".txt";
                break;
            case C::USADDR:
                datasetFile += "usaddr/usaddr" + sizeSufix + ".txt";
                queryFile += "usaddr/q17_" + tau + datasetSuffix + ".txt";
                break;
            case C::MEDLINE19:
                datasetFile += "medline19/medline19" + sizeSufix + ".txt";
                queryFile += "medline19/q17_" + tau + datasetSuffix + ".txt";
                break;
            case C::DBLP:
                datasetFile += "dblp/dblp" + sizeSufix + ".txt";
                queryFile += "dblp/q17_" + tau + datasetSuffix + ".txt";
                break;
            case C::UMBC:
                datasetFile += "umbc/umbc" + sizeSufix + ".txt";
                queryFile += "umbc/q17_" + tau + datasetSuffix + ".txt";
                break;
            case C::JUSBRASIL:
                datasetFile += "jusbrasil/jusbrasil" + sizeSufix + ".txt";
                queryFile += "jusbrasil/q.txt";
                relevantQueryFile += "jusbrasil/relevant_answers.txt";
                break;
            default:
                datasetFile += "aol/aol" + sizeSufix + ".txt";
                queryFile += "aol/q17_" + tau + datasetSuffix + ".txt";
                break;
        }

        readData(datasetFile, records);
//    sort(records.begin(), records.end());
        readData(queryFile, this->queries);
        if (config["has_relevant_queries"] == "1") {
            readData(relevantQueryFile, this->relevantQueries);
        }

        this->trie = new BurstTrie();
        this->trie->buildDaatIndex();

        this->beva = new Beva(this->trie, this->editDistanceThreshold);

        auto done = chrono::high_resolution_clock::now();

#ifdef BEVA_IS_COLLECT_MEMORY_H
        experiment->getMemoryUsedInIndexing();
#else
        experiment->endIndexingTime();
        experiment->compileProportionOfBranchingSizeInBEVA2Level();
        experiment->compileNumberOfNodes();
#endif
        cout << "<<<Index time: " << chrono::duration_cast<chrono::milliseconds>(done - start).count() << " ms>>>\n";
    }

    vector<char *> Framework::processFullQuery(string &query, int queryPosition) {
#ifdef BEVA_IS_COLLECT_TIME_H
        experiment->initQueryProcessingTime();
#endif

        vector<ActiveNode> currentActiveNodes;
        vector<ActiveNode> oldActiveNodes;

        unsigned bitmaps[CHAR_SIZE];
        for (auto &bitmap : bitmaps) bitmap = this->beva->bitmapZero;

        for (int currentPrefixQuery = 1; currentPrefixQuery <= query.size(); currentPrefixQuery++) {
            swap(oldActiveNodes, currentActiveNodes);
            currentActiveNodes.clear();
            this->beva->process(query[currentPrefixQuery - 1], currentPrefixQuery, oldActiveNodes,
                                currentActiveNodes, bitmaps);
            currentActiveNodes.shrink_to_fit();
            oldActiveNodes.clear();
        }

#ifdef BEVA_IS_COLLECT_TIME_H
        experiment->endSimpleQueryProcessingTime(currentActiveNodes.size());
        experiment->initQueryFetchingTime();
#endif

        vector<char *> results = this->output(currentActiveNodes);

#ifdef BEVA_IS_COLLECT_TIME_H
        experiment->endSimpleQueryFetchingTime(results.size());

        bool relevantReturned = false;
        if (queryPosition != -1 && config["has_relevant_queries"] == "1") {
            vector<string> v_output;
            v_output.resize(results.size());
            copy(results.begin(), results.end(), v_output.begin());

            relevantReturned = find(v_output.begin(), v_output.end(),
                                    this->relevantQueries[queryPosition]) != v_output.end();
        }
        experiment->compileSimpleQueryProcessingTimes(query, relevantReturned);
#endif

#ifdef BEVA_IS_COLLECT_MEMORY_H
        experiment->getMemoryUsedInProcessing();
#endif

        return results;
    }

    vector<char *> Framework::processQuery(string &query, int queryId) {
        vector<ActiveNode> currentActiveNodes;
        vector<ActiveNode> oldActiveNodes;

        unsigned bitmaps[CHAR_SIZE];
        for (auto &bitmap : bitmaps) bitmap = this->beva->bitmapZero;

        for (int currentPrefixQuery = 1; currentPrefixQuery <= query.size(); currentPrefixQuery++) {
            swap(oldActiveNodes, currentActiveNodes);
            currentActiveNodes.clear();
            this->process(query, currentPrefixQuery, queryId, oldActiveNodes, currentActiveNodes, bitmaps);
            oldActiveNodes.clear();
        }

        vector<char *> results = this->output(currentActiveNodes);

        return results;
    }

    void Framework::process(string query, int prefixQueryLength, int currentCountQuery,
                            vector<ActiveNode> &oldActiveNodes, vector<ActiveNode> &currentActiveNodes,
                            unsigned (&bitmaps)[CHAR_SIZE]) {
        if (query.empty()) return;

#ifdef BEVA_IS_COLLECT_TIME_H
        experiment->initQueryProcessingTime();
#endif

        this->beva->process(query[prefixQueryLength - 1], prefixQueryLength, oldActiveNodes, currentActiveNodes,
                            bitmaps);

#ifdef BEVA_IS_COLLECT_TIME_H
        experiment->endQueryProcessingTime(currentActiveNodes.size(), prefixQueryLength);

        vector<int> prefixQuerySizeToFetching = {5, 9, 13, 17};
        if (std::find(prefixQuerySizeToFetching.begin(), prefixQuerySizeToFetching.end(), prefixQueryLength) !=
            prefixQuerySizeToFetching.end()) {
            experiment->initQueryFetchingTime();
            vector<char *> results = output(currentActiveNodes);
            experiment->endQueryFetchingTime(prefixQueryLength, results.size());
        }
#endif

        currentActiveNodes.shrink_to_fit();
        if (query.length() == prefixQueryLength) {
#ifdef BEVA_IS_COLLECT_MEMORY_H
            experiment->getMemoryUsedInProcessing();
#else
            experiment->compileQueryProcessingTimes(currentCountQuery);
            string currentQuery = query.substr(0, prefixQueryLength);
            experiment->saveQueryProcessingTime(currentQuery, currentCountQuery);
#endif
        }
    }

    void Framework::writeExperiments() {
#ifdef BEVA_IS_COLLECT_COUNT_OPERATIONS_H
        experiment->compileNumberOfActiveNodes();
            experiment->compileNumberOfIterationInChildren();
#endif
    }

    void transverseTrie(AccessTrie *accessTrie, vector<char *> &outputs, int &limit) {
        if (accessTrie->getEmptyStringPointer().second != -1) {
            outputs.push_back(records[accessTrie->getEmptyStringPointer().second].c_str());
        }
        for (BurstTrieComponent *btcChild : accessTrie->pointers) {
            auto *accessTrieChild = dynamic_cast<AccessTrie *>(btcChild);

            if (accessTrieChild == nullptr) {
                auto *container = dynamic_cast<Container *>(btcChild);
                if (container != nullptr) {
                    for (auto &itr : container->redBlackTree) {
                        outputs.push_back(records[itr.second].c_str());
//                if (outputs.size() >= limit) return outputs;
                    }
                }
                continue;
            } else {
                transverseTrie(accessTrieChild, outputs, limit);
            }
        }
    }

    vector<char *> Framework::output(vector<ActiveNode> &currentActiveNodes) {
        vector<char *> outputs;
        string tmp;
        int limit = 100;

        for (ActiveNode activeNode : currentActiveNodes) {
            bool isRecordBurst = activeNode.burstTrieComponent == nullptr;

            if (isRecordBurst) {
                outputs.push_back(records[activeNode.recordId].c_str());
                string t = records[activeNode.recordId].c_str();
//            if (outputs.size() >= limit) return outputs;
            } else {
                auto *accessTrie = dynamic_cast<AccessTrie *>(activeNode.burstTrieComponent);

                if (accessTrie != nullptr) {
                    transverseTrie(accessTrie, outputs, limit);
                } else {
                    auto *container = dynamic_cast<Container *>(activeNode.burstTrieComponent);

                    for (auto &itr : container->redBlackTree) {
                        int recordId = itr.second;
                        outputs.push_back(records[recordId].c_str());
                        string t = records[recordId].c_str();
//                if (outputs.size() >= limit) return outputs;
                    }
                }
            }
        }

        return outputs;
    }

}

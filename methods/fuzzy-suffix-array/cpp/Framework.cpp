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

namespace fuzzy_suffix_array {

    Experiment *experiment;
    vector<string> records;

    Framework::Framework() {
        this->editDistanceThreshold = stoi(config["edit_distance"]);
        this->dataset = stoi(config["dataset"]);
        experiment = new Experiment(config, stoi(config["edit_distance"]));

        index();
    }

    Framework::~Framework() {
        cout << "deleting framework" << endl;
        delete this->suffixArray;
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

    void Framework::readData(string &filename, vector<string> &recs, bool insertEndOfWord) {
        cout << "reading dataset " << filename << endl;

        string str;
        ifstream input(filename, ios::in);
        while (getline(input, str)) {
            if (insertEndOfWord) str += "$";
            if (!str.empty()) recs.push_back(str);
        }
    }

    void Framework::index() {
        cout << "indexing... \n";
        string sizeSuffix = "";
        switch (stoi(config["size_type"])) {
            case 0:
                sizeSuffix = "_20";
                break;
            case 1:
                sizeSuffix = "_40";
                break;
            case 2:
                sizeSuffix = "_60";
                break;
            case 3:
                sizeSuffix = "_80";
                break;
            case 4:
                sizeSuffix = "";
                break;
            default:
                sizeSuffix = "";
        }

        auto start = chrono::high_resolution_clock::now();

#ifdef IS_COLLECT_TIME_H
        experiment->initIndexingTime();
#endif

        string datasetFile = config["dataset_basepath"];
        string queryFile = config["query_basepath"];

        int queriesSize = stoi(config["queries_size"]);
        string datasetSuffix = queriesSize == 10 ? "_10" : "";
        string tau = to_string(this->editDistanceThreshold);

        switch (this->dataset) {
            case C::AOL:
                datasetFile += "aol/aol" + sizeSuffix + ".txt";
                queryFile += "aol/q17_" + tau + datasetSuffix + ".txt";
                break;
            case C::MEDLINE:
                datasetFile += "medline/medline" + sizeSuffix + ".txt";
                queryFile += "medline/q13" + datasetSuffix + ".txt";
                break;
            case C::USADDR:
                datasetFile += "usaddr/usaddr" + sizeSuffix + ".txt";
                queryFile += "usaddr/q17_" + tau + datasetSuffix + ".txt";
                break;
            case C::MEDLINE19:
                datasetFile += "medline19/medline19" + sizeSuffix + ".txt";
                queryFile += "medline19/q17_" + tau + datasetSuffix + ".txt";
                break;
            case C::DBLP:
                datasetFile += "dblp/dblp" + sizeSuffix + ".txt";
                queryFile += "dblp/q17_" + tau + datasetSuffix + ".txt";
                break;
            case C::UMBC:
                datasetFile += "umbc/umbc" + sizeSuffix + ".txt";
                queryFile += "umbc/q17_" + tau + datasetSuffix + ".txt";
                break;
            case C::JUSBRASIL:
                datasetFile += "jusbrasil/jusbrasil" + sizeSuffix + ".txt";
                queryFile += "jusbrasil/q.txt";
                break;
            default:
                cout << "Dataset not found." << endl;
                exit(1);
                break;
        }

        readData(datasetFile, records, true);
        //    sort(this->records.begin(), this->records.end());
        readData(queryFile, this->queries);

        this->suffixArray = new SuffixArray(this->editDistanceThreshold);
        this->suffixArray->build();

        auto done = chrono::high_resolution_clock::now();

#ifdef IS_COLLECT_MEMORY_H
        experiment->getMemoryUsedInIndexing();
#else
        experiment->endIndexingTime();
#endif
        cout << "<<<Index time: " << chrono::duration_cast<chrono::milliseconds>(done - start).count() << " ms>>>\n";
    }

    vector<string> Framework::processQuery(string &query, int queryId) {
        vector<string> results;
        vector<int> prefixQuerySizes = {5, 9, 13, 17};
        string prefix = query;

        for (int prefixQuerySize : prefixQuerySizes) {
            prefix = query.substr(0, prefixQuerySize);

            if (prefix.size() == prefixQuerySize) {
#ifdef IS_COLLECT_TIME_H
                experiment->initQueryProcessingTime();
#endif
                unordered_map<int, int> resultsMap;
                this->suffixArray->approximateSearch(prefix, resultsMap);

#ifdef IS_COLLECT_TIME_H
                experiment->endQueryProcessingTime(prefixQuerySize);
                experiment->initQueryFetchingTime();
                results = output(resultsMap);
                experiment->endQueryFetchingTime(prefixQuerySize, results.size());
#endif
            }
        }

#ifdef IS_COLLECT_MEMORY_H
        experiment->getMemoryUsedInProcessing();
#else
        experiment->compileQueryProcessingTimes(queryId);
        experiment->saveQueryProcessingTime(prefix, queryId);
#endif

        return results;
    }

    vector<string> Framework::processFullQuery(string &query) {
#ifdef IS_COLLECT_TIME_H
        experiment->initQueryProcessingTime();
#endif

        unordered_map<int, int> resultsMap;
        this->suffixArray->approximateSearch(query, resultsMap);

#ifdef IS_COLLECT_TIME_H
        experiment->endSimpleQueryProcessingTime();
        experiment->initQueryFetchingTime();
#endif

        vector<string> results = this->output(resultsMap);

#ifdef IS_COLLECT_TIME_H
        experiment->endSimpleQueryFetchingTime(results.size());
        experiment->compileSimpleQueryProcessingTimes(query);
#endif

#ifdef IS_COLLECT_MEMORY_H
        experiment->getMemoryUsedInProcessing();
#endif

        return results;
    }

    vector<string> Framework::output(const unordered_map<int, int> &resultsMap) {
        vector<string> results;
        results.reserve(resultsMap.size());

        for (const auto &result : resultsMap) {
            results.emplace_back(records[result.first].c_str());
        }

        return results;
    }

}

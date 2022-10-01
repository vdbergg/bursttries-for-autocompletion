#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "header/Framework.h"
#ifndef BEVA_IS_MAC_H
#include "../../server/crow_all.h"
#endif

using namespace std;

namespace beva_bursttrie_dynamic {

    unordered_map<string, string> config;
    void loadConfigFile();
    void loadConfig(int argc, char** argv);
    Framework* framework;


    void processingQueriesOutsideServer() {
        cout << "processing...\n";

        int indexMin = stoi(config["qry_number_start"]);
        int indexMax = stoi(config["qry_number_end"]);
        indexMax = min(indexMax, (int) framework->queries.size());

        #ifdef BEVA_IS_COLLECT_MEMORY_H
        indexMax = 1000;
        #endif

        if (config["is_full_query_instrumentation"] == "0") {
            for (int i = indexMin; i < indexMax; ++i) {
                framework->processQuery(framework->queries[i], i);
            }
        } else {
            for (int i = indexMin; i < indexMax; ++i) {
                framework->processFullQuery(framework->queries[i], i);
            }
        }
    }

    void processingQueriesInServer() {
        #ifndef BEVA_IS_MAC_H
        crow::SimpleApp app;

        CROW_ROUTE(app, "/autocomplete")
                ([](const crow::request& req) {
                    std::ostringstream os;
                    vector<char *> results;

                    os << "Params: " << req.url_params << "\n\n";

                    if (req.url_params.get("query") != nullptr) {
                        string query = boost::lexical_cast<string>(req.url_params.get("query"));
                        os << "The value of 'query' is " <<  query << '\n';
                        results = framework->processFullQuery(query);
                    }

                    crow::json::wvalue response;
                    response["results"] = results;
                    cout << "<<<< Response length >>>> " + to_string(results.size()) << endl;

                    return response;
//                return crow::response{os.str()};
                });

        // ignore all log
        crow::logger::setLogLevel(crow::LogLevel::Debug);
        //crow::logger::setHandler(std::make_shared<ExampleLogHandler>());

        app.port(18080)
//      .concurrency(10)
                .multithreaded()
                .run();
        #endif
    }


    int main(int argc, char** argv) {
        if (argc == 2) {
            loadConfigFile();
        } else {
            loadConfig(argc, argv);
        }

        framework = new Framework();

        if (config["is_server"] == "0") {
            processingQueriesOutsideServer();
        } else {
            processingQueriesInServer();
        }

        delete framework;
        return 0;
    }

    void loadConfigFile() {
        ifstream is_file("./methods/beva-bursttrie-dynamic/path.cfg");
        string line;

        while (getline(is_file, line)){
            istringstream is_line(line);
            string key;

            if (getline(is_line, key, '=')) {
                string value;

                if (getline(is_line, value)) {
                    config[key] = value;
                }
            }
        }
    }

    void loadConfig(int argc, char** argv) {
        if (argc != 15) {
            cout << "The number of params is incorrect. See path.cfg file from this method for more details." << endl;
            exit(1);
        }

        config["edit_distance"] = argv[2];
        config["dataset"] = argv[3];
        config["queries_size"] = argv[4];
        config["qry_number_start"] = argv[5];
        config["qry_number_end"] = argv[6];
        config["size_type"] = argv[7];
        config["alg"] = "B2LV_DYNAMIC";
        config["dataset_basepath"] = argv[8];
        config["query_basepath"] = argv[9];
        config["experiments_basepath"] = argv[10];
        config["index_level_threshold"] = argv[11];
        config["max_second_level_size"] = argv[12];
        config["is_server"] = argv[13];
        config["is_full_query_instrumentation"] = argv[14];
        config["has_relevant_queries"] = "0";
    }
}

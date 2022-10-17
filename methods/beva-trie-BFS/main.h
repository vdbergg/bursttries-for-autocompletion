/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include <iostream>
#include <string>
#include "header/Framework.h"
#include "header/Directives.h"
#include "../../server/crow_all.h"

using namespace std;

namespace beva_trie_bfs {

    unordered_map<string, string> config;

    void loadConfigFile();

    void loadConfig(int argc, char **argv);

    Framework *framework;


    void processingQueriesOutsideServer() {
        cout << "processing...\n";

        int indexMin = stoi(config["qry_number_start"]);
        int indexMax = stoi(config["qry_number_end"]);

#ifdef BEVA_IS_COLLECT_MEMORY_H
        indexMax = 100;
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
        crow::SimpleApp app;

        CROW_ROUTE(app, "/autocomplete")
                ([](const crow::request &req) {
                    std::ostringstream os;
                    vector<char *> results;

                    os << "Params: " << req.url_params << "\n\n";

                    if (req.url_params.get("query") != nullptr) {
                        string query = boost::lexical_cast<string>(req.url_params.get("query"));
                        os << "The value of 'query' is " << query << '\n';
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
    }


    int main(int argc, char **argv) {
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

#ifdef BEVA_IS_COLLECT_COUNT_OPERATIONS_H
        framework->writeExperiments();
#endif

        delete framework;
        return 0;
    }

    void loadConfigFile() {
        ifstream is_file("./methods/beva-trie-BFS/path.cfg");
        string line;

        while (getline(is_file, line)) {
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

    void loadConfig(int argc, char **argv) {
        if (argc != 13) {
            cout << "The number of params is incorrect. See path.cfg file from this method for more details." << endl;
            exit(1);
        }

        config["edit_distance"] = argv[2];
        config["dataset"] = argv[3];
        config["queries_size"] = argv[4];
        config["qry_number_start"] = argv[5];
        config["qry_number_end"] = argv[6];
        config["size_type"] = argv[7];
        config["alg"] = "BEVA_BFS";
        config["dataset_basepath"] = argv[8];
        config["query_basepath"] = argv[9];
        config["experiments_basepath"] = argv[10];
        config["is_server"] = argv[11];
        config["is_full_query_instrumentation"] = argv[12];
        config["has_relevant_queries"] = "0";
    }

}

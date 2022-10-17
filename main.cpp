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
#include "methods/fuzzy-suffix-array/main.h"
#include "methods/beva-bursttrie-dynamic/main.h"
#include "methods/beva-bursttrie-BFS/main.h"
#include "methods/beva-cpt-BFS/main.h"
#include "methods/beva-trie-BFS/main.h"
#include "methods/beva-trie-DFS/main.h"
#include "methods/beva-trie-dynamic/main.h"

using namespace std;


int main(int argc, char** argv) {
    if (argc > 1) {
        int alg = stoi(argv[1]);
        cout << "alg: ";

        switch (alg) {
            case 1:
                cout << "FUZZY-SUFFIX-ARRAY" << endl;
                return fuzzy_suffix_array::main(argc, argv);
            case 2:
                cout << "BEVA-BURSTTRIE-DYNAMIC" << endl;
                return beva_bursttrie_dynamic::main(argc, argv);
            case 3:
                cout << "BEVA-BURSTTRIE-BFS" << endl;
                return beva_bursttrie_bfs::main(argc, argv);
            case 4:
                cout << "BEVA-CPT-BFS" << endl;
                return beva_cpt_bfs::main(argc, argv);
            case 5:
                cout << "BEVA-TRIE-BFS" << endl;
                return beva_trie_bfs::main(argc, argv);
            case 6:
                cout << "BEVA-TRIE-DFS" << endl;
                return beva_trie_dfs::main(argc, argv);
            case 7:
                cout << "BEVA-TRIE-DYNAMIC" << endl;
                return beva_trie_dynamic::main(argc, argv);
            default:
                return 0;
        }
    } else {
        cout << "Need to pass the algorithm number." << endl;
    }

    return 0;
}

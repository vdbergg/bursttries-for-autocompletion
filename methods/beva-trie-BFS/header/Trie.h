/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include <string>
#include "Node.h"
#include "Experiment.h"
#include "StaticString.h"

#ifndef BEVA_TRIE_BFS_TRIE_H
#define BEVA_TRIE_BFS_TRIE_H

using namespace std;

namespace beva_trie_bfs {

    extern vector<StaticString> records;

    class Trie {
    public:
        vector<Node> globalMemory;
        unsigned root;
        vector<unsigned> lastNodeKnownPerRecord;

        Experiment *experiment;

        Trie(Experiment *);

        void buildDaatIndex();

        unsigned insert(char ch, int, unsigned);

        inline Node *getNodeAddr(unsigned x) {
            return &globalMemory[x];
        }

        inline Node &getNode(unsigned x) {
            if (x >= globalMemory.size()) {
                cout << "ERRO\n";
                exit(1);
            }
            return globalMemory[x];
        }

        inline unsigned newNode() {
            globalMemory.emplace_back();
            return globalMemory.size() - 1;
        }

        inline unsigned newNode(char ch) {
            globalMemory.emplace_back(ch);
            return globalMemory.size() - 1;
        }

        void shrinkToFit();
    };

}

#endif //BEVA_TRIE_BFS_TRIE_H

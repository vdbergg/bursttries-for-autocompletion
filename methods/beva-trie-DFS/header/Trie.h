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

#ifndef BEVA_TRIE_DFS_TRIE_H
#define BEVA_TRIE_DFS_TRIE_H

using namespace std;

namespace beva_trie_dfs {

    extern vector<StaticString> records;

    class Trie {
    public:
        Node root;
        Experiment *experiment;

        Trie(Experiment *);

        void buildKaatIndex();

        Node *insert(char ch, int, Node *);
    };

}

#endif //BEVA_TRIE_DFS_TRIE_H

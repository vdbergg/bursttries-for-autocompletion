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

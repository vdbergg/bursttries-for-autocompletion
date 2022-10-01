#include <string>
#include "Node.h"
#include "Experiment.h"
#include "StaticString.h"

#ifndef BEVA_TRIE_DYNAMIC_TRIE_H
#define BEVA_TRIE_DYNAMIC_TRIE_H

using namespace std;

namespace beva_trie_dynamic {

    extern vector<StaticString> records;

    class Trie {
    public:
        Node root;
        Experiment *experiment;

        Trie(Experiment *);

        void build();

        Node *insert(char ch, Node *);
    };

}

#endif //BEVA_TRIE_DYNAMIC_TRIE_H

/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#ifndef BEVA_TRIE_DFS_NODE_H
#define BEVA_TRIE_DFS_NODE_H


#include <unordered_map>
#include <iostream>
#include <vector>
#include "ShortVector.h"
#include <vector>
#include "EditVector.h"
#include "State.h"

#define CHAR_SIZE 128

using namespace std;

namespace beva_trie_dfs {

    class Node {
    public:
        ShortVector<Node> children;
        unsigned beginRange;
        unsigned endRange;

        Node(char value) {
            this->children.init();
            this->setValue(value);
            this->setIsEndOfWord(false);
        }

        Node() {
            this->children.init();
            this->setIsEndOfWord(false);
        }

        inline void setValue(char value) { this->children.value = value; }

        inline char getValue() { return this->children.value; }

        inline void setIsEndOfWord(bool value) { this->children.isEndOfWord = value; }

        inline bool getIsEndOfWord() { return this->children.isEndOfWord; }

        inline unsigned getBeginRange() { return this->beginRange; }

        inline unsigned getEndRange() { return this->endRange; }

        inline void setBeginRange(unsigned value) { this->beginRange = value; }

        inline void setEndRange(unsigned value) { this->endRange = value; }
    };

}

#endif //BEVA_TRIE_DFS_NODE_H

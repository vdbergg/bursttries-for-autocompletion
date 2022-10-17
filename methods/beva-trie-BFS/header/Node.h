/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#ifndef BEVA_TRIE_BFS_NODE_H
#define BEVA_TRIE_BFS_NODE_H


#include <iostream>
#include "EditVector.h"
#include "State.h"

#define CHAR_SIZE 128

using namespace std;

namespace beva_trie_bfs {

    class Node {
    public:
        //    ShortVector<unsigned> children;
        unsigned children;
        unsigned beginRange;
        unsigned endRange;
        unsigned char numChildren;
        bool isEndOfWord;
        char value;

        Node(char value) {
            this->numChildren = 0;
            this->setValue(value);
            this->setIsEndOfWord(false);
        }

        Node() {
            this->numChildren = 0;
            this->setIsEndOfWord(false);
        }

        inline void setValue(char value) { this->value = value; }

        inline char getValue() { return this->value; }

        inline void setIsEndOfWord(bool value) { this->isEndOfWord = value; }

        inline bool getIsEndOfWord() { return this->isEndOfWord; }

        inline unsigned getBeginRange() { return this->beginRange; }

        inline unsigned getEndRange() { return this->endRange; }

        inline void setBeginRange(unsigned value) { this->beginRange = value; }

        inline void setEndRange(unsigned value) { this->endRange = value; }
    };

}

#endif //BEVA_TRIE_BFS_NODE_H

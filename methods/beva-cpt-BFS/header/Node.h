/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#ifndef BEVA_CPT_BFS_NODE_H
#define BEVA_CPT_BFS_NODE_H


#include <iostream>
#include "EditVector.h"
#include "State.h"
#include "StaticString.h"

using namespace std;

namespace beva_cpt_bfs {

#define CHAR_SIZE 128

    class Node {
    public:
        unsigned children;
        unsigned beginRange = -1;
        unsigned endRange;
        unsigned char numChildren;
        bool isLeaf;
        bool isEndOfWord;
        StaticString label;

        Node() {
            this->numChildren = 0;
            this->setIsLeaf(false);
        }

        Node(StaticString str) {
            this->label = str;
            this->numChildren = 0;
            this->setIsLeaf(false);
        }

        inline char getValue() { return this->label[0]; }

        inline void setIsLeaf(bool value) { this->isLeaf = value; }

        inline bool getIsLeaf() { return this->isLeaf; }

        inline void setIsEndOfWord(bool value) { this->isEndOfWord = value; }

        inline bool getIsEndOfWord() { return this->isEndOfWord; }

        inline unsigned getBeginRange() { return this->beginRange; }

        inline unsigned getEndRange() { return this->endRange; }

        inline void setBeginRange(unsigned value) { this->beginRange = value; }

        inline void setEndRange(unsigned value) { this->endRange = value; }
    };

}

#endif //BEVA_CPT_BFS_NODE_H

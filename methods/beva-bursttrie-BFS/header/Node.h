#ifndef BEVA_BURSTTRIE_BFS_NODE_H
#define BEVA_BURSTTRIE_BFS_NODE_H


#include <iostream>
#include "EditVector.h"
#include "State.h"

using namespace std;

namespace beva_bursttrie_bfs {

#define CHAR_SIZE 128

    class Node {
    public:
        unsigned children;
        unsigned beginRange;
        unsigned endRange;
        unsigned char numChildren;
        bool isLead;
        char value;

        Node(char value) {
            this->numChildren = 0;
            this->setValue(value);
            this->setIsLeaf(false);
        }

        Node() {
            this->numChildren = 0;
            this->setIsLeaf(false);
        }

        inline void setValue(char value) { this->value = value; }

        inline char getValue() { return this->value; }

        inline void setIsLeaf(bool value) { this->isLead = value; }

        inline bool getIsLeaf() { return this->isLead; }

        inline unsigned getBeginRange() { return this->beginRange; }

        inline unsigned getEndRange() { return this->endRange; }

        inline void setBeginRange(unsigned value) { this->beginRange = value; }

        inline void setEndRange(unsigned value) { this->endRange = value; }
    };

}

#endif //BEVA_BURSTTRIE_BFS_NODE_H

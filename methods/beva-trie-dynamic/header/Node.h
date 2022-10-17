/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#ifndef BEVA_TRIE_DYNAMIC_NODE_H
#define BEVA_TRIE_DYNAMIC_NODE_H


#include <unordered_map>
#include <iostream>
#include <vector>
#include "ShortVector.h"
#include <vector>
#include "EditVector.h"
#include "State.h"


using namespace std;

namespace beva_trie_dynamic {
#define CHAR_SIZE 128

class Node {
    public:
        ShortVector<Node> children;

        Node(char value) {
            this->children.init();
            this->setValue(value);
            this->setRecordId(-1);
        }

        Node() {
            this->children.init();
            this->setRecordId(-1);
        }

        inline void setValue(char value) { this->children.value = value; }

        inline char getValue() { return this->children.value; }

        inline void setRecordId(int recordId) { this->children.recordId = recordId; }

        inline int getRecordId() { return this->children.recordId; }
    };

}

#endif //BEVA_TRIE_DYNAMIC_NODE_H

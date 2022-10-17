/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#ifndef BEVA_BURSTTRIE_DYNAMIC_CONTAINER_H
#define BEVA_BURSTTRIE_DYNAMIC_CONTAINER_H

#include <algorithm>
#include <map>
#include "StaticString.h"
#include "BurstTrieComponent.h"
#include "AccessTrie.h"
#include "Directives.h"
#include "GlobalVariables.h"

using namespace std;

namespace beva_bursttrie_dynamic {

    class Container : public virtual BurstTrieComponent {
    public:
        map<string, int> redBlackTree;
        AccessTrie *accessTrieParent;
        int maxLimit;
        unsigned char character;

        Container(int maxLimit, char character, AccessTrie *accessTrieParent);

        ~Container();

        void insertRecordNode(const pair<string, int> &recordNode);

        void burstContainer();

        static void burstContainer(Container *container);

        static void addRecordsNodeAsChildFromNewAccessTrie(map<string, int> &redBlackTree, AccessTrie *newAccessTrie);

        void clear();

        bool isFull() const;

        AccessTrie *getAccessTrieParent() const;

        void setAccessTrieParent(AccessTrie *accessTrie);

        unsigned char getCharacter() const;

        void setCharacter(unsigned char character);

        int getMaxLimit() const;

        void setMaxLimit(int maxLimit);
    };

}

#endif //BEVA_BURSTTRIE_DYNAMIC_CONTAINER_H

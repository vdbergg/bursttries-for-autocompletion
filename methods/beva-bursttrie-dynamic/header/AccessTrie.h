/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#ifndef BEVA_BURSTTRIE_DYNAMIC_ACCESSTRIE_H
#define BEVA_BURSTTRIE_DYNAMIC_ACCESSTRIE_H

#define ALPHABET_SIZE 128

#include <string>
#include "BurstTrieComponent.h"

using namespace std;

namespace beva_bursttrie_dynamic {

    class AccessTrie : public virtual BurstTrieComponent {
    public:
        BurstTrieComponent *pointers[ALPHABET_SIZE];
        pair<string, int> emptyStringPointer;
        unsigned char character;

        AccessTrie();

        AccessTrie(unsigned char character);

        ~AccessTrie();

        BurstTrieComponent *getComponentForPosition(unsigned char character);

        AccessTrie *getTrieForPosition(unsigned char character);

        bool isCharPointerAvailable(unsigned char character);

        void setPointer(unsigned char ch, BurstTrieComponent *btc);

        unsigned char getCharacter() const;

        void setCharacter(unsigned char ch);

        inline void setEmptyStringPointer(
                const pair<string, int> &pEmptyStringPointer) { this->emptyStringPointer = pEmptyStringPointer; }

        inline pair<string, int> getEmptyStringPointer() const { return this->emptyStringPointer; }
    };

}

#endif //BEVA_BURSTTRIE_DYNAMIC_ACCESSTRIE_H

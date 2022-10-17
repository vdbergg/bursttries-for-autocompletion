/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include "../header/AccessTrie.h"

namespace beva_bursttrie_dynamic {

    AccessTrie::AccessTrie() {
        this->character = '\0';
        this->emptyStringPointer = pair<string, int>("", -1);
        for (auto &btc : this->pointers) {
            btc = nullptr;
        }
    }

    AccessTrie::AccessTrie(unsigned char character) {
        this->character = character;
        this->emptyStringPointer = pair<string, int>("", -1);
        for (auto &btc : this->pointers) {
            btc = nullptr;
        }
    }

    AccessTrie::~AccessTrie() {

    }

    void AccessTrie::setPointer(unsigned char ch, BurstTrieComponent *btc) {
        this->pointers[ch] = btc;
    }

    BurstTrieComponent *AccessTrie::getComponentForPosition(unsigned char ch) {
        return this->pointers[ch];
    }

    AccessTrie *AccessTrie::getTrieForPosition(unsigned char ch) {
        return dynamic_cast<AccessTrie *>(this->pointers[ch]);
    }

    bool AccessTrie::isCharPointerAvailable(unsigned char ch) {
        return this->pointers[ch] != nullptr;
    }

    unsigned char AccessTrie::getCharacter() const {
        return this->character;
    }

    void AccessTrie::setCharacter(unsigned char ch) {
        this->character = ch;
    }

}
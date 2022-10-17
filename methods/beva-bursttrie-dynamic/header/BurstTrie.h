/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#ifndef BEVA_BURSTTRIE_DYNAMIC_BURSTTRIE_H
#define BEVA_BURSTTRIE_DYNAMIC_BURSTTRIE_H

#include <vector>
#include "AccessTrie.h"
#include "StaticString.h"
#include "GlobalVariables.h"

namespace beva_bursttrie_dynamic {

    extern vector<StaticString> records;

    class BurstTrie {
    public:
        AccessTrie *rootAccessTrie;

        BurstTrie();

        ~BurstTrie();

        void buildDaatIndex();

        void insertString(BurstTrieComponent *btc, string &str, int recordId);
    };

}

#endif //BBEVA_BURSTTRIE_DYNAMIC_BURSTTRIE_H

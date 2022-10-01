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

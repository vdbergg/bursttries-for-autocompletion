#ifndef BEVA_BURSTTRIE_DYNAMIC_ACTIVENODE_H
#define BEVA_BURSTTRIE_DYNAMIC_ACTIVENODE_H


#include "State.h"
#include "AccessTrie.h"

namespace beva_bursttrie_dynamic {

    class ActiveNode {
    public:
        State *state;
        BurstTrieComponent *burstTrieComponent;
        unsigned level;
        unsigned recordId;

        ActiveNode(BurstTrieComponent*, State*, unsigned, unsigned);
    };

}

#endif //BEVA_BURSTTRIE_DYNAMIC_ACTIVENODE_H

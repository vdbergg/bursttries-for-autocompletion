#ifndef BEVA_TRIE_DYNAMIC_ACTIVENODE_H
#define BEVA_TRIE_DYNAMIC_ACTIVENODE_H

#include "Node.h"
#include "State.h"

namespace beva_trie_dynamic {

    class ActiveNode {
    public:
        State *state;
        Node *node;
        unsigned depth;

        ActiveNode(Node*, State*, unsigned);
    };

}

#endif //BEVA_TRIE_DYNAMIC_ACTIVENODE_H

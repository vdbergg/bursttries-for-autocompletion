#ifndef BEVA_TRIE_BFS_ACTIVENODE_H
#define BEVA_TRIE_BFS_ACTIVENODE_H


#include "State.h"

namespace beva_trie_bfs {

    class ActiveNode {
    public:
        State *state;
        unsigned node;
        unsigned level;

        ActiveNode(unsigned, State *, unsigned);
    };

}

#endif //BEVA_TRIE_BFS_ACTIVENODE_H

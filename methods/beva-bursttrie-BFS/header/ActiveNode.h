#ifndef BEVA_BURSTTRIE_BFS_ACTIVENODE_H
#define BEVA_BURSTTRIE_BFS_ACTIVENODE_H


#include "Node.h"

namespace beva_bursttrie_bfs {

    class ActiveNode {
    public:
        State *state;
        unsigned node;
        unsigned recordId;
        unsigned level;

        ActiveNode(unsigned, State *, unsigned, unsigned);

        ~ActiveNode();
    };

}

#endif //BEVA_BURSTTRIE_BFS_ACTIVENODE_H

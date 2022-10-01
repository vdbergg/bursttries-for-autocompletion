#ifndef BEVA_CPT_BFS_ACTIVENODE_H
#define BEVA_CPT_BFS_ACTIVENODE_H


#include "State.h"

namespace beva_cpt_bfs {

    class ActiveNode {
    public:
        State *state;
        unsigned node;
        unsigned depth;
        unsigned ithCharFromLabel;

        ActiveNode(unsigned, State*, unsigned, int);
    };

}

#endif //BEVA_CPT_BFS_ACTIVENODE_H

#include "../header/ActiveNode.h"

namespace beva_cpt_bfs {

    ActiveNode::ActiveNode(unsigned node, State *state, unsigned depth, int ithCharFromLabel) {
        this->node = node;
        this->state = state;
        this->depth = depth;
        this->ithCharFromLabel = ithCharFromLabel;
    }

}

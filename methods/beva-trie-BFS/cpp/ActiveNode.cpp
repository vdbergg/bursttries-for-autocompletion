#include "../header/ActiveNode.h"

namespace beva_trie_bfs {

    ActiveNode::ActiveNode(unsigned node, State *state, unsigned level) {
        this->node = node;
        this->state = state;
        this->level = level;
    }

}

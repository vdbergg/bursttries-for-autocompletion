#include "../header/ActiveNode.h"

namespace beva_trie_dynamic {

    ActiveNode::ActiveNode(Node *node, State *state, unsigned depth) {
        this->node = node;
        this->state = state;
        this->depth = depth;
    }

}

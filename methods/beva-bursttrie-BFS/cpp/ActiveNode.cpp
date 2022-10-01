#include "../header/ActiveNode.h"

namespace beva_bursttrie_bfs {

    ActiveNode::ActiveNode(unsigned node, State* state, unsigned level, unsigned recordId) {
        this->state = state;
        this->node = node;
        this->level = level;
        this->recordId = recordId;
    }

    ActiveNode::~ActiveNode() {
    }

}

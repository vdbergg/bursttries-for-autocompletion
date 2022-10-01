#include "../header/ActiveNode.h"

namespace beva_bursttrie_dynamic {

    ActiveNode::ActiveNode(BurstTrieComponent *burstTrieComponent, State *state, unsigned level, unsigned recordId) {
        this->burstTrieComponent = burstTrieComponent;
        this->state = state;
        this->level = level;
        this->recordId = recordId;
    }

}

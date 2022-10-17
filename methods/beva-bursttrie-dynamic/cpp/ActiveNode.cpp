/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include "../header/ActiveNode.h"

namespace beva_bursttrie_dynamic {

    ActiveNode::ActiveNode(BurstTrieComponent *burstTrieComponent, State *state, unsigned level, unsigned recordId) {
        this->burstTrieComponent = burstTrieComponent;
        this->state = state;
        this->level = level;
        this->recordId = recordId;
    }

}

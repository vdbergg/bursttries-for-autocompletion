/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


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

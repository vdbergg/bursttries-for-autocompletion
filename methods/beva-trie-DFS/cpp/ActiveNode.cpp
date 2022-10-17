/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include "../header/ActiveNode.h"

namespace beva_trie_dfs {

    ActiveNode::ActiveNode(Node *node, State *state, unsigned depth) {
        this->node = node;
        this->state = state;
        this->depth = depth;
    }

}

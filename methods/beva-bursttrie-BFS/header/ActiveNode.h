/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


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

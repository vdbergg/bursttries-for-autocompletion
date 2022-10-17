/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include "../header/State.h"
#include <utility>

namespace beva_trie_dfs {

    State::State(EditVector *editVector, int id, bool isInitial, bool isFinal) {
        this->editVector = editVector;
        this->isInitial = isInitial;
        this->isFinal = isFinal;
        this->id = id;
    }

    State::~State() {
        delete this->editVector;
    }

}

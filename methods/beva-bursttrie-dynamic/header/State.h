/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#ifndef BEVA_STATE_H
#define BEVA_STATE_H

#include <unordered_map>
#include "EditVector.h"

using namespace std;

namespace beva_bursttrie_dynamic {

    class State {
    public:
        State(EditVector *editVector, int id, bool isInitial = false, bool isFinal = false);

        ~State();

        EditVector *editVector;
        unordered_map<unsigned, State *> transitions;
        bool isInitial;
        bool isFinal;
        int id;


        inline unsigned getEditDistance(int k) {
            return this->editVector->getEditDistance(k);
        }

        inline VectorChar display() {
            return this->editVector->display();
        }
    };

}

#endif //BEVA_STATE_H

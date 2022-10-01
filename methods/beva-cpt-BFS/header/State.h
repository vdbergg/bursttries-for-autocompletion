#ifndef BEVA_CPT_BFS_STATE_H
#define BEVA_CPT_BFS_STATE_H

#include <unordered_map>
#include "EditVector.h"

using namespace std;

namespace beva_cpt_bfs {

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

#endif //BEVA_CPT_BFS_STATE_H

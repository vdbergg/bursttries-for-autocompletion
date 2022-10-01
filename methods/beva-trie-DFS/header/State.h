#ifndef BEVA_TRIE_DFS_STATE_H
#define BEVA_TRIE_DFS_STATE_H

#include <unordered_map>
#include "EditVector.h"

using namespace std;

namespace beva_trie_dfs {

    class State {
    public:
        State(EditVector *editVector, int id, bool isInitial = false, bool isFinal = false);

        ~State();

        EditVector *editVector;
        unordered_map<unsigned, State *> transitions;
        int id;
        bool isInitial;
        bool isFinal;


        inline unsigned getEditDistance(int k) {
            return this->editVector->getEditDistance(k);
        }

        inline VectorChar display() {
            return this->editVector->display();
        }
    };

}

#endif //BEVA_TRIE_DFS_STATE_H

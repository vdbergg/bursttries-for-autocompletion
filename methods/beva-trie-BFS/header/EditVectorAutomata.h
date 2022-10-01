#ifndef BEVA_TRIE_BFS_EDITVECTORAUTOMATON_H
#define BEVA_TRIE_BFS_EDITVECTORAUTOMATON_H

#include <unordered_map>
#include "State.h"

namespace beva_trie_bfs {

    class EditVectorAutomata {
    public:
        State *initialState;
        State *finalState;
        int editDistanceThreshold;
        int size;
        int transitionsByState;

        EditVectorAutomata(int);

        ~EditVectorAutomata();

        State *setTransition(State *&, unsigned, unordered_map<VectorChar, State *, MyHashVectorFunction> &);

        void buildAutomaton();
    };

}

#endif //BEVA_TRIE_BFS_EDITVECTORAUTOMATON_H

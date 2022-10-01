#ifndef BEVA_TRIE_DFS_BEVA_H
#define BEVA_TRIE_DFS_BEVA_H


#include "EditVectorAutomata.h"
#include "Trie.h"
#include "ActiveNode.h"
#include "utils.h"

namespace beva_trie_dfs {

#define CHAR_SIZE 128

    class Beva {
    public:
        Trie *trie;
        EditVectorAutomata *editVectorAutomata;
        Experiment *experiment;
        int editDistanceThreshold;

        int bitmapSize;
        unsigned bitmapZero;

        Beva(Trie *, Experiment *, int);

        ~Beva();

        void process(char, int, vector<ActiveNode> &oldActiveNodes, vector<ActiveNode> &currentActiveNodes,
                     unsigned (&bitmaps)[CHAR_SIZE]);

        void findActiveNodes(unsigned, ActiveNode &, vector<ActiveNode> &, unsigned (&bitmaps)[CHAR_SIZE]);

        inline unsigned buildBitmap(unsigned queryLength, unsigned lastPosition, char c, unsigned (&bitmaps)[CHAR_SIZE]) {
            int k = (int) queryLength - (int) lastPosition;
            return utils::leftShiftBitInDecimal(bitmaps[c], this->editDistanceThreshold - k, this->bitmapSize);
        }

        inline State *getNewState(unsigned queryLength, State *state, unsigned lastPosition, char c,
                                  unsigned (&bitmaps)[CHAR_SIZE]) {
            unsigned bitmap = this->buildBitmap(queryLength, lastPosition, c, bitmaps);

            State *newState = state->transitions[bitmap];
            if (newState == nullptr) newState = state;
            return newState;
        }

        void updateBitmap(char, unsigned (&bitmaps)[CHAR_SIZE]);
    };

}

#endif //BEVA_TRIE_DFS_BEVA_H

#ifndef BEVA_BURSTTRIE_DYNAMIC_BEVA_H
#define BEVA_BURSTTRIE_DYNAMIC_BEVA_H


#include "EditVectorAutomata.h"
#include "ActiveNode.h"
#include "utils.h"
#include "BurstTrie.h"
#include "Container.h"
#include "GlobalVariables.h"

namespace beva_bursttrie_dynamic {

#define CHAR_SIZE 128

    class Beva {
    public:
        BurstTrie *trie;
        EditVectorAutomata *editVectorAutomata;
        int editDistanceThreshold;

        int bitmapSize;
        unsigned bitmapZero;

        Beva(BurstTrie *, int);

        ~Beva();

        void process(char, int, vector<ActiveNode> &oldActiveNodes, vector<ActiveNode> &currentActiveNodes,
                     unsigned (&bitmaps)[CHAR_SIZE]);

        void findActiveAccessTrie(unsigned queryLength, ActiveNode &oldActiveNode, vector<ActiveNode> &activeNodes,
                                  unsigned (&bitmaps)[CHAR_SIZE]);

        void findActiveRecordBurst(unsigned queryLength, ActiveNode &oldActiveNode, vector<ActiveNode> &activeNodes,
                                   unsigned (&bitmaps)[CHAR_SIZE]);

        void findActiveRecordBurstFromActiveAccessTrie(unsigned queryLength, ActiveNode &oldActiveNode,
                                                       vector<ActiveNode> &activeNodes, unsigned (&bitmaps)[CHAR_SIZE]);

        inline unsigned buildBitmap(unsigned queryLength, unsigned lastPosition, unsigned char c,
                                    unsigned (&bitmaps)[CHAR_SIZE]) const {
            int k = (int) queryLength - (int) lastPosition;
            return utils::leftShiftBitInDecimal(bitmaps[c], this->editDistanceThreshold - k, this->bitmapSize);
        }

        inline State *getNewState(unsigned queryLength, State *state, unsigned lastPosition, unsigned char c,
                                  unsigned (&bitmaps)[CHAR_SIZE]) const {
            unsigned bitmap = this->buildBitmap(queryLength, lastPosition, c, bitmaps);

            State *newState = state->transitions[bitmap];
            if (newState == nullptr) newState = state;
            return newState;
        }

        void updateBitmap(char, unsigned (&bitmaps)[CHAR_SIZE]);
    };

}

#endif //BEVA_BURSTTRIE_DYNAMIC_BEVA_H

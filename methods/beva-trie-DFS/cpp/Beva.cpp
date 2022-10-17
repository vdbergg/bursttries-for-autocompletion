/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include "../header/Beva.h"

using namespace std;

namespace beva_trie_dfs {

    Beva::Beva(Trie *trie, Experiment *experiment, int editDistanceThreshold) {
        this->editDistanceThreshold = editDistanceThreshold;
        this->bitmapSize = (1 << ((2 * this->editDistanceThreshold) + 1)) - 1; // 2^(2tau + 1) - 1
        this->trie = trie;
        this->editVectorAutomata = new EditVectorAutomata(this->editDistanceThreshold);
        this->editVectorAutomata->buildAutomaton();
        this->bitmapZero = 0;
        this->experiment = experiment;
    }

    Beva::~Beva() {
        delete this->editVectorAutomata;
    }

    void Beva::process(char ch, int prefixQueryLength, vector<ActiveNode> &oldActiveNodes,
                       vector<ActiveNode> &currentActiveNodes, unsigned (&bitmaps)[CHAR_SIZE]) {
        this->updateBitmap(ch, bitmaps);

        if (prefixQueryLength == 1) {
            currentActiveNodes.emplace_back(&this->trie->root, this->editVectorAutomata->initialState, 0);
#ifdef BEVA_IS_COLLECT_COUNT_OPERATIONS_H
            this->experiment->incrementNumberOfActiveNodes(query.length());
#endif
        } else if (prefixQueryLength > this->editDistanceThreshold) {
            for (ActiveNode oldActiveNode : oldActiveNodes) {
                this->findActiveNodes(prefixQueryLength, oldActiveNode, currentActiveNodes, bitmaps);
            }
        } else {
            swap(currentActiveNodes, oldActiveNodes);
        }
    }

    void
    Beva::updateBitmap(char ch, unsigned (&bitmaps)[CHAR_SIZE]) { // query is equivalent to Q' with the last character c
        for (auto &bitmap : bitmaps) {
            bitmap = utils::leftShiftBitInDecimal(bitmap, 1, this->bitmapSize);
        }

        bitmaps[ch] = bitmaps[ch] | 1;
    }

    void Beva::findActiveNodes(unsigned queryLength, ActiveNode &oldActiveNode,
                               vector<ActiveNode> &activeNodes, unsigned (&bitmaps)[CHAR_SIZE]) {
        unsigned tempSize = oldActiveNode.depth + 1;
        ShortVector<Node>::iterator child = oldActiveNode.node->children.begin();

        for (; child != oldActiveNode.node->children.end(); child++) {

#ifdef BEVA_IS_COLLECT_COUNT_OPERATIONS_H
            this->experiment->incrementNumberOfIterationInChildren(queryLength);
#endif

            State *newState = this->getNewState(queryLength, oldActiveNode.state, tempSize, (*child).getValue(),
                                                bitmaps);

            if (newState->isFinal) continue;

#ifdef BEVA_IS_COLLECT_COUNT_OPERATIONS_H
            this->experiment->incrementNumberOfActiveNodes(queryLength);
#endif

            if (newState->getEditDistance((int) queryLength - (int) tempSize) <= this->editDistanceThreshold) {
                activeNodes.emplace_back(&(*child), newState, tempSize);
            } else {
                ActiveNode tmp(&(*child), newState, tempSize);
                this->findActiveNodes(queryLength, tmp, activeNodes, bitmaps);
            }
        }
    }

}

#include <iostream>
#include "../header/Beva.h"
#include "../header/ActiveNode.h"
#include "../header/utils.h"
#include "../header/Directives.h"

using namespace std;

namespace beva_cpt_bfs {

    Beva::Beva(Trie *trie, Experiment *experiment, int editDistanceThreshold) {
        this->editDistanceThreshold = editDistanceThreshold;
        this->bitmapSize = (1 << ((2 * this->editDistanceThreshold) + 1)) - 1; // 2^(2tau + 1) - 1
        this->trie = trie;
        this->editVectorAutomata = new EditVectorAutomata(this->editDistanceThreshold);
        this->editVectorAutomata->buildAutomaton();
        this->bitmapZero = 0;
        this->bitmapOne = 1;
        this->experiment = experiment;
    }

    Beva::~Beva() {
        delete this->editVectorAutomata;
    }

    void Beva::process(char ch, int prefixQueryLength, vector<ActiveNode> &oldActiveNodes,
                       vector<ActiveNode> &currentActiveNodes, unsigned (&bitmaps)[CHAR_SIZE]) {
        this->updateBitmap(ch, bitmaps);

        if (prefixQueryLength == 1) {
            currentActiveNodes.push_back(ActiveNode(this->trie->root, this->editVectorAutomata->initialState, 0, 0));
#ifdef BEVA_IS_COLLECT_COUNT_OPERATIONS_H
            this->experiment->incrementNumberOfActiveNodes(prefixQueryLength);
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
    Beva::updateBitmap(char ch, unsigned (&bitmaps)[CHAR_SIZE]) {
        for (auto &bitmap : bitmaps) {
            bitmap = utils::leftShiftBitInDecimal(bitmap, 1, this->bitmapSize);
        }

        bitmaps[ch] = bitmaps[ch] | 1;
    }

    void Beva::findActiveNodes(unsigned queryLength, ActiveNode &oldActiveNode,
                               vector<ActiveNode> &activeNodes, unsigned (&bitmaps)[CHAR_SIZE]) {
        auto &curNode = trie->getNode(oldActiveNode.node);

        if (curNode.label.size() == oldActiveNode.ithCharFromLabel) {
            unsigned child = curNode.children;
            unsigned endChilds = child + curNode.numChildren;

            for (; child < endChilds; child++) {
                addChar(queryLength, oldActiveNode, activeNodes, child, this->trie->getNode(child).getValue(), 1,
                        bitmaps);
            }
        } else {
            addChar(queryLength, oldActiveNode, activeNodes, oldActiveNode.node,
                    curNode.label[oldActiveNode.ithCharFromLabel], oldActiveNode.ithCharFromLabel + 1, bitmaps);
        }
    }

    void Beva::addChar(unsigned queryLength, ActiveNode &oldActiveNode, vector<ActiveNode> &activeNodes,
                       unsigned newNode, char ch, int ithCharFromLabel, unsigned (&bitmaps)[CHAR_SIZE]) {
#ifdef BEVA_IS_COLLECT_COUNT_OPERATIONS_H
        this->experiment->incrementNumberOfIterationInChildren(queryLength);
#endif

        State *newState = this->getNewState(queryLength, oldActiveNode.state, oldActiveNode.depth + 1, ch, bitmaps);
        if (newState->isFinal) return;
        int newDepth = (oldActiveNode.depth + 1);

#ifdef BEVA_IS_COLLECT_COUNT_OPERATIONS_H
        this->experiment->incrementNumberOfActiveNodes(queryLength);
#endif
        if (newState->getEditDistance((int) queryLength - (int) newDepth) <= this->editDistanceThreshold) {
            activeNodes.push_back(ActiveNode(newNode, newState, newDepth, ithCharFromLabel));
        } else {
            ActiveNode tmp(newNode, newState, newDepth, ithCharFromLabel);
            this->findActiveNodes(queryLength, tmp, activeNodes, bitmaps);
        }
    }

}

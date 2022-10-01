#include "../header/Beva.h"
#include "../header/ActiveNode.h"
#include "../header/Directives.h"

using namespace std;

namespace beva_bursttrie_bfs {

    const unsigned MAX_UNSIGNED = 0xffffffff;

    Beva::Beva(Trie *trie, int editDistanceThreshold, Experiment *experiment) {
        this->experiment = experiment;
        this->editDistanceThreshold = editDistanceThreshold;
        this->bitmapSize = (1 << ((2 * this->editDistanceThreshold) + 1)) - 1; // 2^(2tau + 1) - 1
        this->trie = trie;
        this->editVectorAutomata = new EditVectorAutomata(this->editDistanceThreshold);
        this->editVectorAutomata->buildAutomaton();
        this->bitmapZero = 0;
        this->bitmapOne = 1;
    }

    Beva::~Beva() {
        delete this->editVectorAutomata;
    }

    void Beva::process(char ch, int prefixQueryLength, vector<ActiveNode> &oldActiveNodes,
                       vector<ActiveNode> &currentActiveNodes, unsigned (&bitmaps)[CHAR_SIZE]) {
        this->updateBitmap(ch, bitmaps);
        if (prefixQueryLength == 1) {
            currentActiveNodes.push_back(ActiveNode(this->trie->root, this->editVectorAutomata->initialState, 0,
                                                    MAX_UNSIGNED));
#ifdef IS_COLLECT_COUNT_OPERATIONS_H
            this->experiment->incrementNumberOfActiveNodes(prefixQueryLength);
#endif
        } else if (prefixQueryLength > this->editDistanceThreshold) {
            for (ActiveNode oldActiveNode : oldActiveNodes) {
                if (oldActiveNode.node == MAX_UNSIGNED) {
                    this->findActiveVirtualNodes(prefixQueryLength, oldActiveNode, currentActiveNodes, bitmaps);
                } else {
#ifdef IS_B2LV_METHOD_H
                    if (this->trie->getNode(oldActiveNode.node).getIsLeaf()) {
                        this->findActiveVirtualNodeFromActiveNodes(prefixQueryLength, oldActiveNode,
                                                                   currentActiveNodes, bitmaps);
                    } else {
                        this->findActiveNodes(prefixQueryLength, oldActiveNode, currentActiveNodes, bitmaps);
                    }
#else
                    if (oldActiveNode.level >= this->trie->indexLevelThreshold) {
                        this->findActiveVirtualNodeFromActiveNodes(prefixQueryLength, oldActiveNode,
                                currentActiveNodes, bitmaps);
                    } else {
                        this->findActiveNodes(prefixQueryLength, oldActiveNode, currentActiveNodes, bitmaps);
                    }
#endif
                }
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

    void Beva::findActiveNodes(unsigned queryLength, ActiveNode &oldActiveNode, vector<ActiveNode> &activeNodes,
                               unsigned (&bitmaps)[CHAR_SIZE]) {
        unsigned child = this->trie->getNode(oldActiveNode.node).children;
        unsigned endChilds = child + this->trie->getNode(oldActiveNode.node).numChildren;

        unsigned tempSize = oldActiveNode.level + 1;
        for (; child < endChilds; child++) {
#ifdef IS_COLLECT_COUNT_OPERATIONS_H
            this->experiment->incrementNumberOfIterationInChildren(queryLength);
#endif

            State *newState = this->getNewState(queryLength, oldActiveNode.state, tempSize,
                                                this->trie->getNode(child).getValue(), bitmaps);

            if (newState->isFinal) continue;

            if (newState->getEditDistance((int) queryLength - (int) tempSize) <= this->editDistanceThreshold) {
                activeNodes.push_back(ActiveNode(child, newState, tempSize, MAX_UNSIGNED));
#ifdef IS_COLLECT_COUNT_OPERATIONS_H
                this->experiment->incrementNumberOfActiveNodes(queryLength);
#endif
            } else {
                ActiveNode tmp(child, newState, tempSize, MAX_UNSIGNED);
#ifdef IS_COLLECT_COUNT_OPERATIONS_H
                this->experiment->incrementNumberOfActiveNodes(queryLength);
#endif

#ifdef IS_B2LV_METHOD_H
                if (this->trie->getNode(child).getIsLeaf()) {
                    findActiveVirtualNodeFromActiveNodes(queryLength, tmp, activeNodes, bitmaps);
                } else {
                    findActiveNodes(queryLength, tmp, activeNodes, bitmaps);
                }
#else
                if (tempSize >= this->trie->indexLevelThreshold) {
                    findActiveVirtualNodeFromActiveNodes(queryLength, tmp, activeNodes, bitmaps);
                } else {
                    findActiveNodes(queryLength, tmp, activeNodes, bitmaps);
                }
#endif
            }
        }
    }

    void Beva::findActiveVirtualNodeFromActiveNodes(unsigned queryLength, ActiveNode &oldActiveNode,
                                                    vector<ActiveNode> &activeNodes, unsigned (&bitmaps)[CHAR_SIZE]) {
        unsigned node = oldActiveNode.node;
        unsigned beginRange = this->trie->getNode(node).getBeginRange();
        unsigned endRange = this->trie->getNode(node).getEndRange();

        for (unsigned i = beginRange; i < endRange; i++) {
            ActiveNode tmp(MAX_UNSIGNED, oldActiveNode.state, oldActiveNode.level, i);
            this->findActiveVirtualNodes(queryLength, tmp, activeNodes, bitmaps);
        }
    }

    void Beva::findActiveVirtualNodes(unsigned queryLength, ActiveNode &oldActiveNode, vector<ActiveNode> &activeNodes,
                                      unsigned (&bitmaps)[CHAR_SIZE]) {
        unsigned tmpLevel = oldActiveNode.level;
        unsigned length = records[oldActiveNode.recordId].length();

        while (tmpLevel < length) {
#ifdef IS_COLLECT_COUNT_OPERATIONS_H
            this->experiment->incrementNumberOfIterationInChildren2Level(queryLength);
#endif

            char ch = records[oldActiveNode.recordId][tmpLevel];
            tmpLevel++;
            oldActiveNode.state = this->getNewState(queryLength, oldActiveNode.state, tmpLevel, ch, bitmaps);

            if (oldActiveNode.state->isFinal) break;

            if (oldActiveNode.state->getEditDistance((int) queryLength - (int) tmpLevel) <=
                this->editDistanceThreshold) {
                activeNodes.push_back(ActiveNode(MAX_UNSIGNED, oldActiveNode.state, tmpLevel, oldActiveNode.recordId));
#ifdef IS_COLLECT_COUNT_OPERATIONS_H
                this->experiment->incrementNumberOfWordActiveNodes(queryLength);
#endif
                break;
            }
        }
    }

}

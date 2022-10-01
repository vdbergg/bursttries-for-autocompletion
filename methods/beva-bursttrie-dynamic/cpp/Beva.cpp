#include "../header/Beva.h"

using namespace std;

namespace beva_bursttrie_dynamic {

    const unsigned MAX_UNSIGNED = 0xffffffff;

    Beva::Beva(BurstTrie *trie, int editDistanceThreshold) {
        this->editDistanceThreshold = editDistanceThreshold;
        this->bitmapSize = (1 << ((2 * this->editDistanceThreshold) + 1)) - 1; // 2^(2tau + 1) - 1
        this->trie = trie;
        this->editVectorAutomata = new EditVectorAutomata(this->editDistanceThreshold);
        this->editVectorAutomata->buildAutomaton();
        this->bitmapZero = 0;
    }

    Beva::~Beva() {
        delete this->editVectorAutomata;
    }

    void Beva::process(char ch, int prefixQueryLength, vector<ActiveNode> &oldActiveNodes,
                       vector<ActiveNode> &currentActiveNodes, unsigned (&bitmaps)[CHAR_SIZE]) {
        this->updateBitmap(ch, bitmaps);

        if (prefixQueryLength == 1) {
            currentActiveNodes.emplace_back(this->trie->rootAccessTrie, this->editVectorAutomata->initialState, 0,
                                            MAX_UNSIGNED);
        } else if (prefixQueryLength > this->editDistanceThreshold) {
            for (ActiveNode oldActiveNode : oldActiveNodes) {
                if (oldActiveNode.burstTrieComponent == nullptr) {
                    this->findActiveRecordBurst(prefixQueryLength, oldActiveNode, currentActiveNodes, bitmaps);
                } else {
                    this->findActiveAccessTrie(prefixQueryLength, oldActiveNode, currentActiveNodes, bitmaps);
                }
            }
        } else {
            swap(currentActiveNodes, oldActiveNodes);
        }
    }

    void Beva::updateBitmap(char ch, unsigned (&bitmaps)[CHAR_SIZE]) {
        for (auto &bitmap : bitmaps) {
            bitmap = utils::leftShiftBitInDecimal(bitmap, 1, this->bitmapSize);
        }

        bitmaps[ch] = bitmaps[ch] | 1;
    }

    void Beva::findActiveAccessTrie(unsigned queryLength, ActiveNode &oldActiveNode,
                                    vector<ActiveNode> &activeNodes, unsigned (&bitmaps)[CHAR_SIZE]) {
        unsigned tempSize = oldActiveNode.level + 1;
        for (BurstTrieComponent *btcChild : dynamic_cast<AccessTrie *>(oldActiveNode.burstTrieComponent)->pointers) {
            auto *accessTrie = dynamic_cast<AccessTrie *>(btcChild);
            if (accessTrie == nullptr) {
                if (dynamic_cast<Container *>(btcChild) != nullptr) {
                    ActiveNode tmp(btcChild, oldActiveNode.state, oldActiveNode.level, MAX_UNSIGNED);
                    this->findActiveRecordBurstFromActiveAccessTrie(queryLength, tmp, activeNodes, bitmaps);
                }
                continue;
            }

            State *newState = this->getNewState(queryLength, oldActiveNode.state, tempSize, accessTrie->getCharacter(),
                                                bitmaps);

            if (newState->isFinal) continue;

            if (newState->getEditDistance((int) queryLength - (int) tempSize) <= this->editDistanceThreshold) {
                activeNodes.emplace_back(btcChild, newState, tempSize, MAX_UNSIGNED);
            } else {
                ActiveNode tmp(btcChild, newState, tempSize, MAX_UNSIGNED);
                this->findActiveAccessTrie(queryLength, tmp, activeNodes, bitmaps);
            }
        }
    }

    void Beva::findActiveRecordBurstFromActiveAccessTrie(unsigned queryLength, ActiveNode &oldActiveNode,
                                                         vector<ActiveNode> &activeNodes,
                                                         unsigned (&bitmaps)[CHAR_SIZE]) {
        auto *container = dynamic_cast<Container *>(oldActiveNode.burstTrieComponent);
        for (auto &itr : container->redBlackTree) {
            ActiveNode tmp(nullptr, oldActiveNode.state, oldActiveNode.level, itr.second);
            this->findActiveRecordBurst(queryLength, tmp, activeNodes, bitmaps);
        }
    }

    void Beva::findActiveRecordBurst(unsigned queryLength, ActiveNode &oldActiveNode, vector<ActiveNode> &activeNodes,
                                     unsigned (&bitmaps)[CHAR_SIZE]) {
        unsigned tmpLevel = oldActiveNode.level;
        unsigned length = records[oldActiveNode.recordId].length();

        while (tmpLevel < length) {
            char ch = records[oldActiveNode.recordId][tmpLevel];
            tmpLevel++;
            oldActiveNode.state = this->getNewState(queryLength, oldActiveNode.state, tmpLevel, ch, bitmaps);

            if (oldActiveNode.state->isFinal) break;

            if (oldActiveNode.state->getEditDistance((int) queryLength - (int) tmpLevel) <=
                this->editDistanceThreshold) {
                activeNodes.emplace_back(nullptr, oldActiveNode.state, tmpLevel, oldActiveNode.recordId);
                break;
            }
        }
    }

}

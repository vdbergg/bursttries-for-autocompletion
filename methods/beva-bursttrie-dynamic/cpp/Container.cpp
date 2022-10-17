/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include "../header/Container.h"
#include "../header/Directives.h"

using namespace std;

namespace beva_bursttrie_dynamic {

    Container::Container(int maxLimit, char character, AccessTrie *accessTrieParent) {
        this->character = character;
        this->accessTrieParent = accessTrieParent;

        switch (maxLimit) {
            case 0:
                this->maxLimit = 10;
                break;
            case 1:
                this->maxLimit = 20;
                break;
            case 2:
                this->maxLimit = 30;
                break;
            case 3:
                this->maxLimit = 40;
                break;
            case 4:
                this->maxLimit = 50;
                break;
            case 5:
                this->maxLimit = 60;
                break;
            case 6:
                this->maxLimit = 70;
                break;
            case 7:
                this->maxLimit = 80;
                break;
            case 8:
                this->maxLimit = 90;
                break;
            case 9:
                this->maxLimit = 100;
                break;
            case 10:
                this->maxLimit = 110;
                break;
            case 11:
                this->maxLimit = 120;
                break;
            case 12:
                this->maxLimit = 130;
                break;
            case 13:
                this->maxLimit = 140;
                break;
            case 14:
                this->maxLimit = 150;
                break;
            case 15:
                this->maxLimit = 160;
                break;
            case 16:
                this->maxLimit = 170;
                break;
            case 17:
                this->maxLimit = 180;
                break;
            case 18:
                this->maxLimit = 190;
                break;
            case 19:
                this->maxLimit = 200;
                break;
            case 20:
                this->maxLimit = 210;
                break;
            case 21:
                this->maxLimit = 220;
                break;
            case 22:
                this->maxLimit = 230;
                break;
            case 23:
                this->maxLimit = 240;
                break;
            case 24:
                this->maxLimit = 250;
                break;
            default:
                this->maxLimit = 0;
                break;
        }
    }

    Container::~Container() = default;

    void Container::insertRecordNode(const pair<string, int> &recordNode) {
        this->redBlackTree.insert(recordNode);

        if (this->isFull()) {
            this->burstContainer();
        }
    }

    void Container::burstContainer() {
        Container::burstContainer(this);
    }

    void Container::burstContainer(Container *container) {
        auto *newAccessTrie = new AccessTrie(container->getCharacter());
        container->getAccessTrieParent()->setPointer(container->getCharacter(), newAccessTrie);

        Container::addRecordsNodeAsChildFromNewAccessTrie(container->redBlackTree, newAccessTrie);
        container->clear();
        delete container;
        container = nullptr;
    }

    void Container::addRecordsNodeAsChildFromNewAccessTrie(map<string, int> &redBlackTree, AccessTrie *newAccessTrie) {
        for (auto &itr : redBlackTree) {
            string recString = itr.first;
            int recordId = itr.second;

            string newValue;

            if (recString.empty()) {
                newAccessTrie->setEmptyStringPointer(pair<string, int>(newValue, recordId));
            } else {
                char leadingChar = recString[0];
                if (recString.size() > 1) newValue = recString.substr(1);

                if (newAccessTrie->isCharPointerAvailable(leadingChar)) {
                    auto *container = dynamic_cast<Container *>(newAccessTrie->getComponentForPosition(leadingChar));
                    container->insertRecordNode(pair<string, int>(newValue, recordId));
                } else {
                    int maxLimit = stoi(config["max_second_level_size"]);
                    auto *newContainer = new Container(maxLimit, leadingChar, newAccessTrie);
                    newContainer->insertRecordNode(pair<string, int>(newValue, recordId));
#ifdef BEVA_IS_COLLECT_TIME_H
                    experiment->incrementNumberOfNodes();
#endif
                    newAccessTrie->setPointer(leadingChar, newContainer);
                }
            }
        }
    }

    void Container::clear() {
        this->redBlackTree.clear();
    }

    bool Container::isFull() const {
        return this->redBlackTree.size() > this->maxLimit && config["alg"] != "B2LF_DYNAMIC";
    }

    AccessTrie *Container::getAccessTrieParent() const {
        return this->accessTrieParent;
    }

    void Container::setAccessTrieParent(AccessTrie *pAccessTrieParent) {
        this->accessTrieParent = pAccessTrieParent;
    }

    unsigned char Container::getCharacter() const {
        return this->character;
    }

    void Container::setCharacter(unsigned char ch) {
        this->character = ch;
    }

    int Container::getMaxLimit() const {
        return this->maxLimit;
    }

    void Container::setMaxLimit(int maxLim) {
        this->maxLimit = maxLim;
    }

}

/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include "../header/Trie.h"
#include "../header/utils.h"
#include "../header/Directives.h"
#include <iostream>

namespace beva_bursttrie_bfs {

    vector<StaticString> records;

    Trie::Trie(int indexLevelThreshold, Experiment *experiment, unordered_map<string, string> &config) {
        this->indexLevelThreshold = indexLevelThreshold;
        this->experiment = experiment;

        this->globalMemory.reserve(records.size() * 3);

        this->root = newNode();
        getNode(this->root).setBeginRange(0);
        getNode(this->root).setEndRange(records.size());

#ifdef IS_COLLECT_TIME_H
        this->experiment->incrementNumberOfNodes();
#endif

        this->lastNodeKnownPerRecord.reserve(records.size());
        for (int recordId = 0; recordId < records.size(); recordId++) {
            this->lastNodeKnownPerRecord[recordId] = this->root;
        }

        switch (stoi(config["max_second_level_size"])) {
            case 0:
                this->maxSecondLevelSize = 10;
                break;
            case 1:
                this->maxSecondLevelSize = 20;
                break;
            case 2:
                this->maxSecondLevelSize = 30;
                break;
            case 3:
                this->maxSecondLevelSize = 40;
                break;
            case 4:
                this->maxSecondLevelSize = 50;
                break;
            case 5:
                this->maxSecondLevelSize = 60;
                break;
            case 6:
                this->maxSecondLevelSize = 70;
                break;
            case 7:
                this->maxSecondLevelSize = 80;
                break;
            case 8:
                this->maxSecondLevelSize = 90;
                break;
            case 9:
                this->maxSecondLevelSize = 100;
                break;
            case 10:
                this->maxSecondLevelSize = 110;
                break;
            case 11:
                this->maxSecondLevelSize = 120;
                break;
            case 12:
                this->maxSecondLevelSize = 130;
                break;
            case 13:
                this->maxSecondLevelSize = 140;
                break;
            case 14:
                this->maxSecondLevelSize = 150;
                break;
            case 15:
                this->maxSecondLevelSize = 160;
                break;
            case 16:
                this->maxSecondLevelSize = 170;
                break;
            case 17:
                this->maxSecondLevelSize = 180;
                break;
            case 18:
                this->maxSecondLevelSize = 190;
                break;
            case 19:
                this->maxSecondLevelSize = 200;
                break;
            case 20:
                this->maxSecondLevelSize = 210;
                break;
            case 21:
                this->maxSecondLevelSize = 220;
                break;
            case 22:
                this->maxSecondLevelSize = 230;
                break;
            case 23:
                this->maxSecondLevelSize = 240;
                break;
            case 24:
                this->maxSecondLevelSize = 250;
                break;
            default:
                this->maxSecondLevelSize = 0;
                break;
        }
    }

    Trie::~Trie() {
    }

    bool Trie::halt(int currentIndexLevel, unsigned node) {
        unsigned beginRange = getNode(node).getBeginRange();
        unsigned endRange = getNode(node).getEndRange();
        unsigned rangeSize = beginRange != 1 && endRange != -1 ? endRange - beginRange : 0;

        if (rangeSize > this->maxSecondLevelSize) return false;
        return currentIndexLevel >= this->indexLevelThreshold;
    }

    void Trie::buildDaatIndex() {
#ifdef IS_B2LV_METHOD_H
        int maxLevel = records[0].length();
#else
        int maxLevel = this->indexLevelThreshold;
#endif

        bool added = true;

        for (int currentIndexLevel = 0; currentIndexLevel < maxLevel && added; currentIndexLevel++) {

            added = false;
            for (int recordId = 0; recordId < records.size(); recordId++) {

                if (currentIndexLevel <= records[recordId].length() - 1) {
                    unsigned parent = this->lastNodeKnownPerRecord[recordId];

#ifdef IS_B2LV_METHOD_H
                    if (records[recordId].length() > maxLevel) {
                        maxLevel = records[recordId].length();
                    }

                    if (this->halt(currentIndexLevel, parent)) {
                        getNode(parent).setIsLeaf(true);
#ifdef IS_COLLECT_TIME_H
                        this->experiment->proportionOfBranchingSizeInBEVA2Level(currentIndexLevel + 1);
#endif
                        continue;
                    }
#endif

                    unsigned char ch = records[recordId][currentIndexLevel];

                    unsigned node = this->search(ch, recordId, parent);
                    getNode(node).setEndRange(recordId + 1);
                    this->lastNodeKnownPerRecord[recordId] = node;
                    added = true;

#ifdef IS_B2LV_METHOD_H
#else
                    if (currentIndexLevel == records[recordId].length() - 1 || currentIndexLevel == maxLevel - 1) {
                        getNode(node).setIsLeaf(true);
#ifdef IS_COLLECT_TIME_H
                            this->experiment->proportionOfBranchingSizeInBEVA2Level(currentIndexLevel + 1);
#endif
                    }
#endif
                }
            }
        }
    }

    unsigned Trie::search(unsigned char ch, int recordId, unsigned parent) {
        unsigned numChildren = getNode(parent).numChildren;
        if (numChildren) {
            unsigned child = getNode(parent).children + numChildren - 1;
            if (getNode(child).getValue() == ch) return child;
        }
        unsigned node = newNode();
        if (numChildren == 0) getNode(parent).children = node;

        getNode(node).setValue(ch);
        getNode(node).setBeginRange(recordId);
        getNode(parent).numChildren++;
#ifdef IS_COLLECT_TIME_H
        this->experiment->incrementNumberOfNodes();
#endif

        return node;
    }

    void Trie::shrinkToFit() {
        this->globalMemory.shrink_to_fit();

        this->lastNodeKnownPerRecord.clear();
        this->lastNodeKnownPerRecord.shrink_to_fit();
    }

}

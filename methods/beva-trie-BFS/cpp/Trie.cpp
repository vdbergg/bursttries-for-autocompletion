/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include "../header/Trie.h"
#include "../header/Experiment.h"
#include "../header/Directives.h"

namespace beva_trie_bfs {

    vector<StaticString> records;

    Trie::Trie(Experiment *experiment) {
        this->experiment = experiment;
        if (this->experiment->config["dataset"] == "3") { // Medline
            this->globalMemory.reserve(2308200000);
        } else {
            this->globalMemory.reserve(records.size() * 4);
        }

        this->root = newNode();
        getNode(this->root).setBeginRange(0);
        getNode(this->root).setEndRange(records.size());
        this->experiment->incrementNumberOfNodes();

        this->lastNodeKnownPerRecord.reserve(records.size());
        for (int recordId = 0; recordId < records.size(); recordId++) {
            this->lastNodeKnownPerRecord[recordId] = this->root;
        }
    }

    void Trie::buildDaatIndex() {
        int maxLevel = records[0].length();

        for (int currentIndexLevel = 0; currentIndexLevel < maxLevel; currentIndexLevel++) {
            for (int recordId = 0; recordId < records.size(); recordId++) {

                if (currentIndexLevel <= records[recordId].length() - 1) {
                    unsigned parent = this->lastNodeKnownPerRecord[recordId];

                    if (records[recordId].length() > maxLevel) {
                        maxLevel = records[recordId].length();
                    }

                    unsigned char ch = records[recordId][currentIndexLevel];
                    unsigned node = this->insert((char) ch, recordId, parent);
                    getNode(node).setEndRange(recordId + 1);
                    this->lastNodeKnownPerRecord[recordId] = node;

                    if (currentIndexLevel == records[recordId].length() - 1) {
                        getNode(node).setIsEndOfWord(true);
#ifdef BEVA_IS_COLLECT_TIME_H
                        this->experiment->proportionOfBranchingSize(currentIndexLevel + 1);
#endif
                    }
                }
            }
        }
    }

    unsigned Trie::insert(char ch, int recordId, unsigned parent) {
        unsigned child;
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
#ifdef BEVA_IS_COLLECT_TIME_H
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

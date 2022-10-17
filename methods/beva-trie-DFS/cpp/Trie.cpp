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

namespace beva_trie_dfs {

    vector<StaticString> records;

    Trie::Trie(Experiment *experiment) {
        this->experiment = experiment;

        this->root.setBeginRange(0);
        this->root.setEndRange(records.size());
        this->experiment->incrementNumberOfNodes();
    }

    void Trie::buildKaatIndex() {
        Node *node;
        for (int recordId = 0; recordId < records.size(); recordId++) {
            node = &this->root;
            int currentIndexDepth = 0;
            for (unsigned char ch : string(records[recordId].c_str())) {
                node = this->insert((char) ch, recordId, node);

                currentIndexDepth++;
                node->setEndRange(recordId + 1);
            }
            node->setIsEndOfWord(true);
#ifdef BEVA_IS_COLLECT_TIME_H
            this->experiment->proportionOfBranchingSize(currentIndexDepth);
#endif
        }
    }

    Node *Trie::insert(char ch, int recordId, Node *parent) {
        ShortVector<Node>::iterator vit = parent->children.begin();

        for (; vit != parent->children.end(); vit++) {
            if ((*vit).getValue() == ch) break;
        }

        if (vit == parent->children.end()) {
            Node node;
            node.setValue(ch);
            node.setBeginRange(recordId);
            parent->children.push_back(node);
#ifdef BEVA_IS_COLLECT_TIME_H
            this->experiment->incrementNumberOfNodes();
#endif
            return &(parent->children[parent->children.getSize() - 1]);
        }

        return &(*vit);
    }

}

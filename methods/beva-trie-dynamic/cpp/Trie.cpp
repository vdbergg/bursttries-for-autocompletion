#include "../header/Trie.h"

namespace beva_trie_dynamic {

    vector<StaticString> records;

    Trie::Trie(Experiment *experiment) {
        this->experiment = experiment;
        this->experiment->incrementNumberOfNodes();
    }

    void Trie::build() {
        Node *node;
        for (int recordId = 0; recordId < records.size(); recordId++) {
            node = &this->root;

            for (unsigned char ch : string(records[recordId].c_str())) {
                node = this->insert((char) ch, node);
            }
            node->setRecordId(recordId);
#ifdef BEVA_IS_COLLECT_TIME_H
            this->experiment->proportionOfBranchingSize(records[recordId].size());
#endif
        }
    }

    Node *Trie::insert(char ch, Node *parent) {
        ShortVector<Node>::iterator vit = parent->children.begin();

        for (; vit != parent->children.end(); vit++) {
            if ((*vit).getValue() == ch) break;
        }

        if (vit == parent->children.end()) {
            Node node;
            node.setValue(ch);
            parent->children.push_back(node);
#ifdef BEVA_IS_COLLECT_TIME_H
            this->experiment->incrementNumberOfNodes();
#endif
            return &(parent->children[parent->children.getSize() - 1]);
        }

        return &(*vit);
    }

}

#include "../header/BurstTrie.h"
#include "../header/Container.h"
#include "../header/Directives.h"

namespace beva_bursttrie_dynamic {

    vector<StaticString> records;

    BurstTrie::BurstTrie() {
        this->rootAccessTrie = new AccessTrie();

#ifdef BEVA_IS_COLLECT_TIME_H
        experiment->incrementNumberOfNodes();
#endif
    }

    BurstTrie::~BurstTrie() {

    }

    void BurstTrie::buildDaatIndex() {
        int maxIndexLevelThreshold = stoi(config["index_level_threshold"]);

        for (int recordId = 0; recordId < records.size(); recordId++) {
            auto *root = this->rootAccessTrie;
            string record = records[recordId].c_str();
            string recordFromTrie = record.substr(0, maxIndexLevelThreshold);

            if (!recordFromTrie.empty()) {

                for (char ch : recordFromTrie) {
                    BurstTrieComponent *nextBtc = root->getComponentForPosition(ch);

                    if (nextBtc == nullptr) {
                        auto *newAccessTrie = new AccessTrie(ch);
                        root->setPointer(ch, newAccessTrie);
                        root = newAccessTrie;
#ifdef BEVA_IS_COLLECT_TIME_H
                        experiment->incrementNumberOfNodes();
#endif
                    } else {
                        root = dynamic_cast<AccessTrie *>(nextBtc);
                    }
                }
            }

            if (maxIndexLevelThreshold < record.size()) {
                string recordFromContainer = record.substr(maxIndexLevelThreshold, record.size());
                this->insertString(root, recordFromContainer, recordId);
            } else {
                root->setEmptyStringPointer(pair<string, int>("", recordId));
            }
        }
    }

    void BurstTrie::insertString(BurstTrieComponent *btc, string &str, int recordId) {
        if (dynamic_cast<Container *>(btc) != nullptr) {
            auto *container = dynamic_cast<Container *>(btc);
            container->insertRecordNode(pair<string, int>(str.c_str(), recordId));
        } else {
            if (str.length() == 0) {
                auto *accessTrie = dynamic_cast<AccessTrie *>(btc);
                accessTrie->setEmptyStringPointer(pair<string, int>(str.c_str(), recordId));
            } else {
                char ch = str[0];
                string newValue;
                if (str.length() > 1) newValue = str.substr(1);

                auto *accessTrie = dynamic_cast<AccessTrie *>(btc);
                BurstTrieComponent *nextBtc = accessTrie->getComponentForPosition(ch);

                if (nextBtc == nullptr) {
                    int maxLimit = stoi(config["max_second_level_size"]);
                    auto *newContainer = new Container(maxLimit, ch, accessTrie);
#ifdef BEVA_IS_COLLECT_TIME_H
                    experiment->incrementNumberOfNodes();
#endif
                    accessTrie->setPointer(ch, newContainer);
                    this->insertString(newContainer, newValue, recordId);
                } else {
                    auto *container = dynamic_cast<Container *>(nextBtc);
                    if (container != nullptr) {
                        container->insertRecordNode(pair<string, int>(newValue.c_str(), recordId));
                    } else {
                        this->insertString(nextBtc, newValue, recordId);
                    }
                }
            }
        }
    }

}

#include <string>
#include "Node.h"
#include "Experiment.h"
#include "StaticString.h"

#ifndef BEVA_CPT_BFS_TRIE_H
#define BEVA_CPT_BFS_TRIE_H

using namespace std;

namespace beva_cpt_bfs {

    extern vector<StaticString> records;

    class Trie {
    public:
        vector<Node> globalMemory;
        unsigned root;
        vector<unsigned> lastNodeKnownPerRecord;
        vector<unsigned> nextIthCharKnownPerRecord;
        Experiment *experiment;

        Trie(Experiment *);

        void buildDaatIndex();

        unsigned insertDaat(int &ithCharFromRecord, int recordId, unsigned parent,
                            unordered_map<unsigned, unsigned> &numberOfChildrenCreated);

        void mergeSinglesChildWithParent(unordered_map<unsigned, unsigned> &numberOfChildrenCreated);

        inline Node *getNodeAddr(unsigned x) {
            return &globalMemory[x];
        }

        inline void removeNode() {
            globalMemory.resize(globalMemory.size() - 1);
        }

        inline Node &getNode(unsigned x) {
            if (x >= globalMemory.size()) {
                cout << "ERROR" << endl;
                exit(1);
            }
            return globalMemory[x];
        }

        inline unsigned newNode() {
            globalMemory.emplace_back();
            return globalMemory.size() - 1;
        }

        inline unsigned newNode(const StaticString &str) {
            globalMemory.emplace_back(str);
            return globalMemory.size() - 1;
        }

        void shrinkToFit();
    };

}

#endif //BEVA_CPT_BFS_TRIE_H

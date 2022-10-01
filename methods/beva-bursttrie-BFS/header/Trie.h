#include <string>
#include "Node.h"
#include "Experiment.h"
#include "StaticString.h"

#ifndef BEVA_BURSTTRIE_BFS_TRIE_H
#define BEVA_BURSTTRIE_BFS_TRIE_H

using namespace std;

namespace beva_bursttrie_bfs {

    extern vector<StaticString> records;

    class Trie {
    public:
        unsigned root;
        int indexLevelThreshold;
        int maxSecondLevelSize;
        Experiment *experiment;

        vector<Node> globalMemory;
        vector<unsigned> lastNodeKnownPerRecord;

        Trie(int, Experiment *experiment, unordered_map<string, string> &);

        ~Trie();

        void buildDaatIndex();

        unsigned search(unsigned char ch, int, unsigned);

        bool halt(int currentIndexLevel, unsigned node);

        inline Node *getNodeAddr(unsigned x) {
            return &this->globalMemory[x];
        }

        inline Node &getNode(unsigned x) {
            if (x >= this->globalMemory.size()) {
                cout << "ERROR\n";
                exit(1);
            }
            return this->globalMemory[x];
        }

        inline unsigned newNode() {
            this->globalMemory.emplace_back();
            return this->globalMemory.size() - 1;
        }

        inline unsigned newNode(unsigned char ch) {
            this->globalMemory.emplace_back(ch);
            return this->globalMemory.size() - 1;
        }

        void shrinkToFit();
    };

}

#endif //BEVA_BURSTTRIE_BFS_TRIE_H

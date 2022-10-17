/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include <string>
#include "C.h"

#ifndef BEVA_BURSTTRIE_BFS_EDITVECTOR_H
#define BEVA_BURSTTRIE_BFS_EDITVECTOR_H

using namespace std;

namespace beva_bursttrie_bfs {

    struct VectorChar {
    public:
        unsigned char *vector;
        unsigned char size;

        VectorChar() {
            this->size = 0;
            this->vector = nullptr;
        }

        VectorChar(int size) {
            this->size = size;
            this->vector = new unsigned char[this->size];
        }

        bool operator==(const VectorChar &vet) const {
            for (int i = 0; i < this->size; i++) {
                if (vet.vector[i] != this->vector[i]) return false;
            }
            return true;
        }

        void set(int position, unsigned char item) {
            this->vector[position] = item;
        }

        unsigned char get(int position) {
            return this->vector[position];
        }
    };

    class MyHashVectorFunction {
    public:
        size_t operator()(const VectorChar &vectorChar) const {
            size_t hashVal;

            switch (vectorChar.size) {
                case 3:
                    return (((*((unsigned short *) vectorChar.vector))) << 8) + (vectorChar.vector[2] & 0xFF);
                case 5:
                    return (*((unsigned *) vectorChar.vector)) + (vectorChar.vector[4] & 0xFF);
                case 7:
                    return (*((unsigned *) vectorChar.vector)) +
                           (((*((unsigned short *) (vectorChar.vector + 1)))) << 8) +
                           (vectorChar.vector[6] & 0xFF);
                case 9:
                    return (*((unsigned long *) vectorChar.vector)) + (vectorChar.vector[8] & 0xFF);
                default: {
                    hashVal = 0;
                    hashVal += (*((unsigned long *) vectorChar.vector));
                    for (int i = 8; i < vectorChar.size; i++) {
                        hashVal += (vectorChar.vector[i] << ((i % 4) << 3));
                    }
                }
            }
            hashVal = hashVal ^ (hashVal << 4) ^ (hashVal << 2);
            return hashVal;
        }
    };

    class EditVector {
    public:
        EditVector(int);

        ~EditVector();

        VectorChar vectorChar;
        int editDistanceThreshold;
        int size;

        void buildInitialEditVector();

        void buildEditVectorWithBitmap(unsigned, EditVector *);

        inline unsigned getEditDistance(int k) {
            int result = (this->editDistanceThreshold + k);
            if (result < this->size) return this->get(result);
            return C::MARKER;
        }

        inline unsigned char get(int position) {
            return this->vectorChar.get(position);
        }

        inline void set(int position, unsigned char item) {
            if (item > this->editDistanceThreshold) this->vectorChar.set(position, '#');
            else this->vectorChar.set(position, item);
        }

        inline VectorChar display() {
            return this->vectorChar;
        }
    };

}

#endif //BEVA_BURSTTRIE_BFS_EDITVECTOR_H

/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#ifndef BEVA_TRIE_DYNAMIC_UTILS_H
#define BEVA_TRIE_DYNAMIC_UTILS_H

#include <vector>
#include <bitset>
#include <iostream>

using namespace std;

namespace beva_trie_dynamic {

    class utils {
    public:
        static void split(string, char, vector<string> &);

        static void printVector(int *, int);

        static void printTransition(int *, int, int *, int, string &);

        static inline int min(int a, int b, int c) {
            return std::min(std::min(a, b), c);
        };

        static inline unsigned leftShiftBitInDecimal(unsigned decimal, unsigned countShiftBit, unsigned bitmapSize) {
            return (decimal << countShiftBit) & bitmapSize;
        };

        static inline unsigned char getKthBitFromDecimal(unsigned int decimal, unsigned k) {
            return (decimal >> k) & 1;
        };
    };

}

#endif //BEVA_TRIE_DYNAMIC_UTILS_H

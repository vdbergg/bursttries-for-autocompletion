#ifndef BEVA_CPT_BFS_UTILS_H
#define BEVA_CPT_BFS_UTILS_H

#include <vector>
#include <bitset>
#include <iostream>

using namespace std;

namespace beva_cpt_bfs {

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

#endif //BEVA_CPT_BFS_UTILS_H

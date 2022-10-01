#ifndef BEVA_BURSTTRIE_BFS_UTILS_H
#define BEVA_BURSTTRIE_BFS_UTILS_H
#include <vector>
#include <iostream>
#include <bitset>
#include <sstream>
#include <string>


using namespace std;

namespace beva_bursttrie_bfs {

    class utils {
    public:
        static void split(string &, char, vector<string> &);

        static void printVector(int *, int);

        static void printTransition(int *, int, int *, int, string &);

        static inline int min(int a, int b, int c) {
            return std::min(std::min(a, b), c);
        }

        static inline unsigned leftShiftBitInDecimal(unsigned decimal, unsigned countShiftBit, unsigned bitmapSize) {
            return (decimal << countShiftBit) & bitmapSize;
        }

        static inline unsigned char getKthBitFromDecimal(unsigned decimal, unsigned short int k) {
            return (decimal >> k) & 1;
        }
    };

}

#endif //BEVA_BURSTTRIE_BFS_UTILS_H

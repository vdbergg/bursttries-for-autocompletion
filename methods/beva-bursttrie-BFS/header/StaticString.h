#ifndef BEVA_BURSTTRIE_BFS_STATICSTRING_H
#define BEVA_BURSTTRIE_BFS_STATICSTRING_H


#include <string>
#include <string.h>
#include <vector>
#include <iostream>

using namespace std;

namespace beva_bursttrie_bfs {

    typedef unsigned long integerPointer;

    extern integerPointer currFreePosition;
    extern integerPointer dataEnd;
    extern char *_data_static;

    class StaticString {
    public:
        // one value per object
        integerPointer dataPos;
        unsigned long stringSize;

        inline string substr(unsigned long start, unsigned long length) {
            string tmp;
            if (start + length <= stringSize) {
                tmp.reserve(length + 1);
                tmp = strncpy((char *) (tmp.c_str()), _data_static + dataPos + start, length);
            } else {
                if (start < stringSize) {
                    tmp.reserve(length + 1);
                    strncpy((char *) (tmp.c_str()), _data_static + dataPos + start, length);
                } else {
                    tmp = "";
                }
            }
            return tmp;
        }

        inline unsigned long length() { return stringSize; }

        inline char operator[](unsigned long pos) { return *(_data_static + dataPos + pos); }

        inline static void setDataBaseMemory(char *ptr, unsigned long size) {
            _data_static = ptr;
            dataEnd = size;
            currFreePosition = 0;
        }

        StaticString(string s) {
            insertNewString(s);
        }

        inline char *c_str() { return _data_static + dataPos; }


        void insertNewString(string s) {
            dataPos = currFreePosition;
            if (dataPos < dataEnd) {
                stringSize = s.length();
                strcpy(_data_static + dataPos, s.c_str());
                currFreePosition += s.length() + 1;
            } else {
                cout << "Insertion error into dataset in-memory" << endl;
                exit(1);
            }
        }
    };

}

#endif //BEVA_BURSTTRIE_BFS_STATICSTRING_H

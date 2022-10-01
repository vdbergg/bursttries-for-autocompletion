#ifndef FUZZY_SUFFIX_ARRAY_STATICSTRING_H
#define FUZZY_SUFFIX_ARRAY_STATICSTRING_H


#include <string>
#include <string.h>
#include <vector>
#include <iostream>

using namespace std;

namespace fuzzy_suffix_array {

    typedef unsigned long integerPointer;


    extern integerPointer currFreePosition;
    extern integerPointer dataEnd;
    extern char *_data_static;

    const unsigned long staticStringNPos = 0xFFFFFFFF;

    class StaticString {
    public:
        integerPointer dataPos;
        unsigned long stringSize;

        inline string substr(unsigned long start = 0, unsigned long length = staticStringNPos) {
            string tmp = "";
            if (length == 0) return tmp;
            if (length > stringSize) length = stringSize;

            if (start + length <= stringSize) {
                tmp.reserve(length + 1);
                tmp = string(_data_static + dataPos).substr(start, length);
            } else {
                if (start < stringSize) {
                    tmp.reserve(length + 1);
                    tmp = string(_data_static + dataPos).substr(start, stringSize - start);
                } else {
                    tmp = "";
                }
            }
            return tmp;
        }

        inline unsigned long length() { return stringSize; }

        inline unsigned long size() { return stringSize; }

        inline char operator[](unsigned long pos) { return *(_data_static + dataPos + pos); }

        inline static void setDataBaseMemory(char *ptr, unsigned long size) {
            _data_static = ptr;
            dataEnd = size;
            currFreePosition = 0;
        }

        StaticString(const string s) {
            insertNewString(s);
        }

        StaticString(const StaticString &s) {
            this->dataPos = s.dataPos;
            this->stringSize = s.stringSize;
        }

        StaticString() {
            this->stringSize = 0;
            this->dataPos = 0;
        }

        StaticString(unsigned long data, unsigned long size) {
            this->dataPos = data;
            this->stringSize = size;
        }

        void resize(unsigned long newSize) {
            if (newSize <= this->stringSize) {
                this->stringSize = newSize;
            } else {
                cout << "static strings cannot be resized to grow." << endl;
                exit(1);
            }
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

#endif //BEVA_STATICSTRING_H

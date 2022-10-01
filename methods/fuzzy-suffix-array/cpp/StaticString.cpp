#include "../header/StaticString.h"

namespace fuzzy_suffix_array {

    integerPointer currFreePosition;
    integerPointer dataEnd;
    char *_data_static;

    bool operator<(const StaticString &x1, const StaticString &x2) {
        return strcmp(_data_static + x1.dataPos, _data_static + x2.dataPos) < 0;
    }

    bool operator==(const StaticString &x1, const StaticString &x2) {
        return strcmp(_data_static + x1.dataPos, _data_static + x2.dataPos) == 0;
    }

}

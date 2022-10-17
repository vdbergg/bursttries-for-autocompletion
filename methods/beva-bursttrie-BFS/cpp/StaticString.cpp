/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#include "../header/StaticString.h"

namespace beva_bursttrie_bfs {

    integerPointer currFreePosition;
    integerPointer dataEnd;
    char *_data_static;

    bool operator < (const StaticString &x1, const StaticString &x2) {
        return strcmp(_data_static + x1.dataPos, _data_static + x2.dataPos) < 0;
    }


    bool operator == (const StaticString &x1, const StaticString &x2) {
        return strcmp(_data_static + x1.dataPos, _data_static + x2.dataPos) == 0;
    }

}

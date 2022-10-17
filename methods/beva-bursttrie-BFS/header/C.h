/*
 * Title: Applying Burst-Tries for Error-Tolerant Prefix Search
 * Authors: Berg Ferreira · Edleno Silva de Moura · Altigran da Silva
 * Journal: Information Retrieval Journal (IRJ)
 * DOI: 10.1007/s10791-022-09416-9
 *
 * Note: For any reference to the repository, please cite this article.
 */


#ifndef BEVA_BURSTTRIE_BFS_C_H
#define BEVA_BURSTTRIE_BFS_C_H

namespace beva_bursttrie_bfs {

    class C {
    public:
        static const int BEVA = 0;

        static const int MARKER = 50;

        static const int AOL = 0;
        static const int MEDLINE = 1;
        static const int USADDR = 2;
        static const int MEDLINE19 = 3;
        static const int DBLP = 4;
        static const int UMBC = 5;
        static const int JUSBRASIL = 6;

    };

}

#endif //BEVA_BURSTTRIE_BFS_C_H

# Applying Burst-Tries for Error-Tolerant Prefix Search

**Authors:** Berg Ferreira · Edleno Silva de Moura · Altigran da Silva

**Journal:** Information Retrieval Journal (IRJ)

**Abstract:** In this work we address the problem of performing an error-tolerant prefix
search on a set of string keys. While the ideas presented here could be adopted in other
applications, our primary target application is error-tolerant query autocompletion.
Tries and their variations have been adopted as the basic data structure to implement 
recently proposed error-tolerant prefix search methods. However, they must require a lot 
of extra memory to process queries. Burst tries are alternative compact tries proposed 
to reduce storage costs and maintain a close performance when compared to the use of tries. 
Here we discuss alternatives for adapting burst tries as error-tolerant prefix search data 
structures. We show how to adapt state-of-the-art trie-based methods for use with burst tries.
We studied the trade-off between memory usage and time performance while
varying the parameters to build the burst trie index. As an example, when indexing
the JusBrasil dataset, one of the datasets adopted in the experiments, the use of burst
tries reduces the memory required by a full trie to 26% and increases the time performance to 116%. 
The possibility of balancing memory usage and time performance
constitutes an advantage of the burst trie when compared to the full trie when adopted
as an index for the task of performing error-tolerant prefix search.

**DOI:** 10.1007/s10791-022-09416-9

**Note:** For any reference to the repository, please cite this article.


## Requirements

1. Boost library
2. Pthread library

## How to use

This repository includes some methods that run different index over the 
[BEVA algorithm.](https://www.research.ed.ac.uk/en/publications/beva-an-efficient-query-processing-algorithm-for-error-tolerant-a)

1. **beva-bursttrie-BFS:** The application of burst trie for BEVA algorithm with the index construction in a BFS direction. 
2. **beva-bursttrie-dynamic:** The application of burst trie for BEVA algorithm with a dynamic index.
3. **beva-cpt-BFS:** The application of *Compact Prefix Tree* for BEVA algorithm with the index construction in a BFS direction.
4. **beva-trie-BFS:** The BEVA algorithm using a full trie with the index construction in a BFS direction.
5. **beva-trie-DFS:** The BEVA algorithm using a full trie with the index construction in a DFS direction. The same algorithm 
   implemented by the BEVA's authors.
6. **beva-trie-dynamic:** The BEVA algorithm using a full trie with a dynamic index.
7. **fuzzy-suffix-array:** The fuzzy prefix search over a suffix array.

### Running a method

For run a method follow the steps:

1. Compile the source code:

> g++ -std=c++11 -I. methods/beva-bursttrie-dynamic/cpp/* methods/fuzzy-suffix-array/cpp/* methods/beva-bursttrie-BFS/cpp/* methods/beva-cpt-BFS/cpp/* methods/beva-trie-BFS/cpp/* methods/beva-trie-DFS/cpp/* methods/beva-trie-dynamic/cpp/* *.cpp -O3 -o output -lboost_system -lpthread

2. Run the binary file using the path.cfg file of the method:

> ./output <algorithm_number>

The path.cfg file is found at the path `methods/<method>/path.cfg`

or 

> ./output <algorithm_number> <edit_distance_threshold> <dataset_number> <queries_size> <qry_number_start> <qry_number_end> <size_type> <dataset_basepath> <query_basepath> <experiments_basepath> <is_server> <is_full_query_instrumentation>

> ./bbd 7 1 0 1000 0 1000 4 /mnt/storage/datasets/autocompletion/ /mnt/storage/datasets/autocompletion/ ./methods/beva-trie-dynamic/experiments/ 0 0

or when run **beva-bursttrie-BFS** method.

> ./output <algorithm_number> <edit_distance_threshold> <dataset_number> <queries_size> <qry_number_start> <qry_number_end> <size_type> <dataset_basepath> <query_basepath> <experiments_basepath> < mcd> < mck> <is_server> <is_full_query_instrumentation>


## Parameters

* **<algorithm_number>**: An integer from 1 to 7, where

1. **fuzzy-suffix-array**
2. **beva-bursttrie-dynamic**
3. **beva-bursttrie-BFS**
    1. MCD: The burst trie heuristic with the maximum depth by container.
       1. Comment the variable `IS_B2LV_METHOD_H` on the file `methods/beva-bursttrie-BFS/header/Directives.h`.
        2. Pass just the **mcd** parameter.
    2. MCK: The burst trie heuristic with the maximum number of key by container.
       1. Uncomment the variable `IS_B2LV_METHOD_H` on the file `methods/beva-bursttrie-BFS/header/Directives.h`.
          2. Pass just the **mck** parameter.
    3. MCD+MCK: The burst trie heuristic with maximum depth by container and the maximum number of key by container.
       1. Uncomment the variable `IS_B2LV_METHOD_H` on the file `methods/beva-bursttrie-BFS/header/Directives.h`.
        2. Pass both **mcd** and **mck** parameters.
4. **beva-cpt-BFS**
5. **beva-trie-BFS**
6. **beva-trie-DFS**
7. **beva-trie-dynamic**

* **<edit_distance_threshold>**: An integer from 1 to 5.
  

* **<<dataset_number>>**: An integer from 0 to 6, where 

0. **AOL**
1. **MEDLINE**
2. **USADDR**
3. **MEDLINE19**
4. **DBLP**
5. **UMBC**
6. **JUSBRASIL**

* **<queries_size>:** An integer indicating the number of queries that will be read from queries file.


* **<qry_number_start>**: An integer indicating the start index in the queries file.
  

* **<qry_number_end>**: An integer indicating the end index in the queries file.


* **<size_type>**: An integer from 0 to 4 indicating the portion of the size of the dataset, where

0. **20%**
1. **40%**
2. **60%**
3. **80%**
4. **100%**

OBS: The portion of the dataset must be generated previously with the correspondent suffix as `aol_20.txt` or `aol.txt` to 100% of the AOL dataset.


* **<dataset_basepath>**: The string base path to the dataset. Into of the base path must have for each dataset a 
  directory with the dataset name and into of this directory must have the dataset file with the name of the dataset and the 
  extension .txt. For example `/mnt/storage/datasets/autocompletion/aol/aol.txt`, the `/mnt/storage/datasets/autocompletion/` 
  is the base path, `aol` is the directory and `aol.txt` is the dataset file.


* **<query_basepath>**: The string base path to the queries. Into of the base path must have for each dataset a
  directory with the dataset name and into of this directory must have the queries file with the name `q.txt`. 
  For example `/mnt/storage/datasets/autocompletion/aol/q.txt`, the `/mnt/storage/datasets/autocompletion/`
  is the base path, `aol` is the directory and `q17_1.txt` is the queries file from the aol dataset with 17 characters 
  to edit distance threshold equal to 1.

* **<experiments_basepath>**: The string with the base path for save the experiments output.


* **< mcd>**: A integer from 2 to until *height of the tree* indicating the burst trie heuristic with the maximum depth by container.


* **< mck>**: A integer from 0 to 24 indicating the burst trie heuristic with the number maximum of keys by container.


* **<is_server>**: A bool with value 0 or 1, being 1 to the method up as a server, or 0 otherwise.
  

* **<is_full_query_instrumentation>**: A bool with value 0 or 1, being 1 to instrument the queries 
  by full query or 0 to the instrumentation be performed character by character, simulating each user's keystroke.

## Datasets

* The datasets must be built with a one suggestion (set of words) by row. 


* The queries must be built synthetically with a query by row extracted from dataset and introduced errors randomly. 

**OBS: In this repo we provide the AOL dataset.**

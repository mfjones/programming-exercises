# Programming exercises

A collection of programming exercises. I've implemented various data
structures in both Java and C++.

## Java

- [Heap implementation](java/HeapImplementation.java).
- [Hash map implementation](java/MapImplementation.java).
- [Undirected graph implementation](java/GraphImplementation.java).
- [A bunch of sorting algorithms](java/Sorting.java).
- See also [this code](https://github.com/mfjones/pace2016), which was submitted to a programming competition for IPEC 2016. The code was also used to produce experimental results for [a paper I co-wrote](https://link.springer.com/article/10.1007/s00453-018-0499-1).

## C++

- [Fibonacci heap](c++/data_structures/fib_heap.h)
- [Union find](c++/data_structures/union_find.h)
- [Undirected graph](c++/data_structures/graph.cpp)
- I've also recently been playing around with [CGAL](https://www.cgal.org/):
  - I am following the exercises in [this book](https://www.springer.com/gp/book/9783642172823) to learn how to develop with CGAL, see [my solutions](c++/cgal/cgal_book).
  - For practice, implemented the brute-force algorithm for computing the extremal yolk for a point set (see the paper I co-wrote: [Fast Algorithms for Geometry Consensuses](https://arxiv.org/abs/1912.01639)). See implementation [here](c++/cgal/yolk/yolk.cpp).
  - Implemented the algorithm from [this paper I co-wrote](https://arxiv.org/abs/1712.02949) for computing the approximate [centerpoint](https://en.wikipedia.org/wiki/Centerpoint_(geometry)) of a point set. See the code [here](c++/cgal/centerpoint).

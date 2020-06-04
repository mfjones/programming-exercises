//
//  main.cpp
//  tests
//
//  Created by Mitchell Jones on 6/1/20.
//  Copyright © 2020 Mitchell Jones. All rights reserved.
//

#include <gtest/gtest.h>

#include "graph.h"
#include "fib_heap.h"
#include "union_find.h"

// NOLINTNEXTLINE
TEST(UnionFindTest, Simple)
{
  UnionFind<int> uf;
  uf.makeSet(1);
  uf.makeSet(2);
  uf.makeSet(3);

  ASSERT_EQ(1, uf.size(1));
  uf.merge(1, 2);
  ASSERT_EQ(2, uf.size(1));
  ASSERT_EQ(2, uf.size(2));
  int expec = uf.find(2);
  // should have same roots if they're in the same set.
  ASSERT_EQ(expec, uf.find(1));
  ASSERT_EQ(3, uf.find(3));
}

// NOLINTNEXTLINE
TEST(FibHeapTest, Sorting) {
  FibHeap<char,int> fh;
  fh.insert('a', -1);
  fh.insert('b', 2);
  fh.insert('c', -10);
  fh.insert('d', 4);
  
  ASSERT_EQ(-10, fh.extractMin().second);
  ASSERT_EQ(-1, fh.extractMin().second);
  ASSERT_EQ(2, fh.extractMin().second);
  ASSERT_EQ(4, fh.extractMin().second);
  
  ASSERT_EQ(0, fh.size());
  
  fh.insert('a', 10);
  fh.insert('b', 11);
  ASSERT_EQ(10, fh.getValue('a'));
  fh.decrease('b', 4);
  ASSERT_EQ('b', fh.minKey());
  ASSERT_EQ(2, fh.size());
  ASSERT_EQ(10, fh.remove('a').second);
  ASSERT_EQ(4, fh.extractMin().second);
  
  ASSERT_EQ(0, fh.size());
}

// NOLINTNEXTLINE
TEST(GraphTest, NeighborSize) {
  Graph G;
  G.addEdge(1, 2);
  G.addEdge(1, 3);
  int expec = 2;
  ASSERT_EQ(expec, G.getNeighbors(1).size());
  ASSERT_EQ(3, G.shortestUnweightedPath(2,3).size());
  ASSERT_EQ(2, G.shortestUnweightedPath(1,2).size());
}

// NOLINTNEXTLINE
TEST(GraphTest, MinimumSpanningTree) {
  Graph G;
  G.addEdge(1, 2, 4);
  G.addEdge(2, 3, -1);
  G.addEdge(3, 1, 2);
  Graph mst = G.minSpanningTree();
  ASSERT_EQ(1, mst.weight());
  ASSERT_EQ(3, mst.numVertices());

  Graph mst2 = mst.minSpanningTree();
  ASSERT_EQ(1, mst.weight());
  ASSERT_EQ(mst.numVertices(), mst2.numVertices());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

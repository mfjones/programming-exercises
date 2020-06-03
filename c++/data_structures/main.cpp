#include <iostream>
#include <string>

#include "circular_list.h"
#include "fib_heap.h"
#include "graph.h"
#include "union_find.h"

using namespace std;

template<typename T>
void print_v(const vector<T> & v) {
  for (auto t : v) {
    cout << t << " ";
  }
}

int main() {
  Graph G;
  G.addEdge(0,1,4);
  G.addEdge(0,7,8);
  G.addEdge(1,7,11);
  G.addEdge(7,8,7);
  G.addEdge(7,6,1);
  G.addEdge(1,2,0);
  G.addEdge(2,8,2);
  G.addEdge(8,6,6);
  G.addEdge(6,5,2);
  G.addEdge(2,3,7);
  G.addEdge(2,5,4);
  G.addEdge(3,5,14);
  G.addEdge(3,4,9);
  G.addEdge(5,4,10);

  Graph MST = G.minSpanningTree();
  cout << endl << "G has MST of weight " << MST.weight() << endl;
  cout << MST << "\n\n";

  vector<int> path = G.shortestUnweightedPath(0, 4);
  cout << "Shortest (unweighted) path from 0 to 4 has vertices: ";
  print_v(path);
  cout << "\n\n";

  Result r = G.shortestPath(0, 4);
  cout << "Minimum weight path from 0 to 4 has vertices: ";
  print_v(r.path);
  cout << "..with weight " << r.weight << endl << endl;

  return 0;
}

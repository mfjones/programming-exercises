//
// Created by Mitchell Jones on 5/28/20.
//

#ifndef DATA_STRUCTURES_GRAPH_H
#define DATA_STRUCTURES_GRAPH_H

#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "fib_heap.h"
#include "union_find.h"

class Edge {
  int weight_;
  std::pair<int, int> edge_;
public:
  Edge(int u, int v, int w = 0) : edge_(u, v), weight_(w) {};
  int weight() const { return weight_; };
  std::pair<int, int> endpoints() const { return edge_; };
  int getOtherEndpoint(int x) const {
    int u = edge_.first; int v = edge_.second;
    if (x == u) return v;
    else if (x == v) return u;
    else return -1;
  };

  bool operator<(const Edge &e) const { return weight_ < e.weight_; };
};

// MARK: -

typedef std::unordered_set<int> VertexSet;
typedef std::vector<Edge> EdgeSet;
typedef std::unordered_map<int, std::unordered_set<int>> NeighborSet;
typedef std::unordered_map<int, EdgeSet> IncidentEdgeSet;

// For returning minimum weight u-v path.
struct Result {
  std::vector<int> path;
  int weight;
};

// MARK: -

class Graph {
  VertexSet vertices_;
  EdgeSet edges_;
  NeighborSet neighbors_;
  IncidentEdgeSet incident_edges_;
public:
  void addVertex(int v);
  void addEdge(int u, int v, int weight = 0);
  VertexSet getNeighbors(int v);
  EdgeSet getIncidentEdges(int v);
  std::vector<int> shortestUnweightedPath(int u, int v);
  Result shortestPath(int u, int v);
  Graph minSpanningTree();
  int weight();
  
  int numVertices() { return int(vertices_.size()); };

  friend std::ostream & operator<<(std::ostream & os, const Graph & G);
};

#endif //DATA_STRUCTURES_GRAPH_H

//
// Created by Mitchell Jones on 5/28/20.
//

#include "graph.h"

constexpr int kInf = std::numeric_limits<int>::max();

// MARK: - Modifiers

void Graph::addVertex(const int& v) {
  vertices_.insert(v);
  std::unordered_set<int> n_v;
  neighbors_[v] = n_v;
}

void Graph::addEdge(const int& u, const int& v, const int& weight) {
  Edge e(u, v, weight);

  auto itv = neighbors_.find(v);
  // Vertex doesn't exist yet in G.
  if (itv == neighbors_.end()) {
    addVertex(v);
  }
  neighbors_[v].insert(u);
  incident_edges_[v].push_back(e);

  auto itu = neighbors_.find(u);
  // Vertex doesn't exist yet in G.
  if (itu == neighbors_.end()) {
    addVertex(u);
  }
  neighbors_[u].insert(v);
  incident_edges_[u].push_back(e);

  edges_.push_back(e);
}

// MARK: - Accessors

VertexSet Graph::getNeighbors(const int& v) const {
  auto it = neighbors_.find(v);
  if (it != neighbors_.end()) {
    return it->second;
  } else {
    throw std::invalid_argument("Vertex " + std::to_string(v) + " doesn't exist!");
  }
}

EdgeSet Graph::getIncidentEdges(const int& v) const {
  auto it = incident_edges_.find(v);
  if (it != incident_edges_.end()) {
    return it->second;
  } else {
    throw std::invalid_argument("Vertex " + std::to_string(v) + " doesn't exist!");
  }
}

// MARK: - Graph operations

// Shortest unweighted path between u and v
std::vector<int> Graph::shortestUnweightedPath(const int& u, const int& v) const {
  // Use a queue to keep track of currently unexplored vertices.
  std::unordered_map<int, bool> seen;
  std::unordered_map<int, int> parent;
  for (auto it : vertices_) {
    seen[it] = false;
  }

  std::queue<int> bfs;
   // u is the root of the bfs tree.
  parent[u] = -1;
  bfs.push(u);
  seen[u] = true;

  bool found = false;
  while (!bfs.empty()) {
    // Go through u's neighbors, add any unseen vertices to the queue
    // and set the parent.
    int curr = bfs.front();
    bfs.pop();

    if (curr == v) {
      found = true;
      break;
    }

    for (auto neighbor : getNeighbors(curr)) {
      if (seen[neighbor]) {
        continue;
      }
      seen[neighbor] = true;
      bfs.push(neighbor);
      parent[neighbor] = curr;
    }
  }

  if (!found) {
    return std::vector<int>();
  }

  std::vector<int> path;
  path.push_back(v);
  int curr = parent[v];
  while (curr != -1) {
    path.push_back(curr);
    curr = parent[curr];
  }

  reverse(path.begin(), path.end());
  return path;
}

Result Graph::shortestPath(const int& u, const int& v) const {
  fib_heap<int, int> dist;
  std::unordered_map<int, int> parents;
  std::unordered_map<int, bool> seen;

  for (auto it : vertices_) {
    dist.insert(it, kInf);
    parents[it] = -1;
    seen[it] = false;
  }
  dist.decrease(u, 0);
  parents[u]= -1; // u is the root of the shortest path tree.

  Result r;

  bool found = false;
  while (!dist.empty()) {
    std::pair<int, int> pr = dist.extractMin();
    int vtx = pr.first;
    int distance = pr.second;
    seen[vtx] = true;

    if (vtx == v) {
      found = true;
      r.weight = distance;
      break;
    }

    for (auto edge : getIncidentEdges(vtx)) {
      int neighbor = edge.getOtherEndpoint(vtx);
      int altDist = distance + edge.weight();
      if (!seen[neighbor] && altDist < dist.getValue(neighbor)) {
        dist.decrease(neighbor, altDist);
        parents[neighbor] = vtx;
      }
    }
  }

  if (!found) {
    return {std::vector<int>(), -1};
  }

  std::vector<int> path;
  path.push_back(v);
  int curr = parents[v];
  while (curr != -1) {
    path.push_back(curr);
    curr = parents[curr];
  }

  std::reverse(path.begin(), path.end());
  r.path = path;
  return r;
}

// Returns MST of G via Kruskal's algorithm and
// using union find data structure.
Graph Graph::minSpanningTree() {
  // Sort edges in increasing weight and initialize
  // union find DS.
  UnionFind<int> uf;
  for (int v : vertices_) {
    uf.makeSet(v);
  }

  Graph MST;

  sort(edges_.begin(), edges_.end());
  for (auto edge : edges_) {
    // Find sets containing both endpoints of the edge. If
    // the lie in different components, merge them and add
    // cost of edge. Otherwise, adding edge could create a cycle.

    int u = edge.endpoints().first;
    int v = edge.endpoints().second;
    int uRoot = uf.find(u);
    int vRoot = uf.find(v);

    if (vRoot == uRoot) continue;

    MST.addEdge(u, v, edge.weight());
    uf.merge(uRoot, vRoot);
  }

  return MST;
}

// MARK: - Utility functions

int Graph::weight() const {
  int weight = 0;
  for (auto edge : edges_) {
    weight += edge.weight();
  }
  return weight;
}

std::ostream & operator<<(std::ostream & os, const Graph & G) {
  os << "Vertices: ";
  VertexSet::iterator it;
  for (it = G.vertices_.begin(); it != G.vertices_.end();) {
    os << *it;
    if (++it != G.vertices_.end()) os << ", ";
  }

  os << std::endl << "Adjacency list: ";
  NeighborSet::const_iterator itm;
  for (itm = G.neighbors_.begin(); itm != G.neighbors_.end(); ) {
    os << "(" << itm->first << ", {";
    VertexSet::iterator it2;
    for (it2 = itm->second.begin(); it2 != itm->second.end(); ) {
      os << *it2;
      if (++it2 != itm->second.end()) os << ", ";
    }
    os << "})";
    if (++itm != G.neighbors_.end()) os << ", ";
  }

  os << std::endl << "Edges: ";
  EdgeSet::const_iterator ite;
  for (ite = G.edges_.begin(); ite != G.edges_.end(); ) {
    os << "(" << ite->endpoints().first
       << ", " << ite->endpoints().second << "; "
       << ite->weight() << ")";
    if (++ite != G.edges_.end()) os << ", ";
  }

  return os;
}

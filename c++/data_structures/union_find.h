//
// Created by Mitchell Jones on 5/28/20.
//

#ifndef DATA_STRUCTURES_UNION_FIND_H
#define DATA_STRUCTURES_UNION_FIND_H

#include <iostream>
#include <map>
#include <vector>

template <typename T>
class UnionFind {
  std::map<T, T> parent_;
  std::map<T, int> sizes_;
  T parent(T &t);
  int size(T &t);

public:
  void makeSet(T &t);
  T find(T &t);
  void merge(T &t1, T &t2);

  template <typename U>
  friend std::ostream &operator<<(std::ostream &os, const UnionFind<U> &uf);
};

template<typename T>
void UnionFind<T>::makeSet(T &t) {
  auto it = parent_.find(t);
  if (it != parent_.end()) return; // t already exists

  parent_[t] = t;
  sizes_[t] = 1;
}

// Returns root containing element t.
template<typename T>
T UnionFind<T>::find(T &t) {
  // Check if t exists.
  if (parent_.find(t) == parent_.end())
    throw std::invalid_argument("Element t does not exist.");

  T root = t;
  while (parent(root) != root) {
    root = parent(root);
  }

  // Perform path compression.
  T temp = t;
  while (parent(temp) != root) {
    T par = parent(temp);
    parent_[temp] = root;
    temp = par;
  }

  return root;
}

template<typename T>
void UnionFind<T>::merge(T &t1, T &t2) {
  T x_root = find(t1);
  T y_root = find(t2);

  // Check if already in same set.
  if (x_root == y_root) return;

  // Merge smaller into larger.
  if (size(x_root) < size(y_root)) {
    parent_[x_root] = y_root;
    sizes_[y_root] = size(y_root) + size(x_root);
    sizes_.erase(x_root);
  } else {
    parent_[y_root] = x_root;
    sizes_[x_root] = size(y_root) + size(x_root);
    sizes_.erase(y_root);
  }
}

template<typename T>
T UnionFind<T>::parent(T &t) {
  return parent_.find(t)->second;
}

// Returns size of set containing t.
template<typename T>
int UnionFind<T>::size(T &t) {
  T root = find(t);
  return sizes_[root];
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const UnionFind<T> &uf) {
  std::map<T, std::vector<T>> trees;
  for (auto it : uf.parent_) {
    T child = it.first;
    T parent = it.second;
    if (trees.find(parent) != trees.end()) {
      trees.find(parent)->second.push_back(child);
    } else {
      std::vector<T> children;
      children.push_back(child);
      trees.insert({parent, children});
    }
  }

  for (auto it : trees) {
    os << "(" << it.first << ") {";
    for (auto it2 = it.second.begin(); it2 != it.second.end();) {
      os << *it2;
      if (++it2 != it.second.end()) os << ", ";
    }
    os << "}";
  }

  return os;
}

#endif //DATA_STRUCTURES_UNION_FIND_H

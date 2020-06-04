//
//  fib_heap.h
//  data_structures
//
//  Created by Mitchell Jones on 6/2/20.
//  Copyright © 2020 Mitchell Jones. All rights reserved.
//

#ifndef fib_heap_h
#define fib_heap_h

#include <iostream>
#include <limits>
#include <map>
#include <stack>
#include <vector>

#include "circular_list.h"

template<typename K, typename V>
struct FibNode {
  K key;
  V value;
  bool mark = false;
  
  FibNode<K, V>* par = NULL;
  
  // Circular list of children.
  CircularList<V, FibNode<K, V>> children;
  int degree() const { return children.size(); };
  
  // Handled implicitly by the circular list.
  FibNode<K, V>* left = NULL;
  FibNode<K, V>* right = NULL;
};

template<typename K, typename V>
class FibHeap {
  CircularList<V, FibNode<K, V>> roots_;
  FibNode<K, V>* min_root_;
  std::map<K, FibNode<K, V>*> map_;
  int size_;
  
  const int kNegInf = std::numeric_limits<V>::min();
  
public:
  FibHeap() : size_(0), min_root_(NULL) {};
  void insert(const K& key, const V& value);
  std::pair<K, V> extractMin();
  void decrease(const K& key, const V& newValue);
  std::pair<K, V> remove(const K& key);
  V& getValue(const K& key) const;
  
  int size() const { return size_; };
  bool empty() const { return size_ == 0; };
  FibNode<K, V>* minRoot() const { return min_root_; };
  K minKey() const { return min_root_->key; };
  
  template<typename U, typename S>
  friend std::ostream& operator<<(std::ostream& os, const FibHeap<U, S>& fh);
  
private:
  void updateMin();
  void consolidate();
  void link(FibNode<K, V>* par, FibNode<K, V>* child);
  void cut(FibNode<K, V>* par, FibNode<K, V>* child);
  void cascadingCut(FibNode<K, V>* node);
};

// MARK: - Public functions

template<typename K, typename V>
void FibHeap<K, V>::insert(const K& key, const V& value) {
  // Construct new node.
  auto* node = new FibNode<K, V>;
  node->key = key;
  node->value = value;
  node->par = NULL;
  node->mark = false;
  
  // Set up the left/right links for the node.
  roots_.insert(node);
  
  // Update min_root ptr if necessary.
  if (size_ == 0 || value < min_root_->value) {
    min_root_ = node;
  }
  size_++;

  // Insert into map for efficient access.
  map_.insert({key, node});
}

template<typename K, typename V>
std::pair<K, V> FibHeap<K, V>::extractMin() {
  if (size() == 0) {
    throw std::out_of_range("The heap is empty!");
  }
  
  // Remove the min from the roots list and capture the ptr.
  FibNode<K, V>* min = min_root_;
  roots_.remove(min, false);
  
  // Add all children to the roots list.
  int deg = min->degree();
  FibNode<K, V>* curr = min->children.node();
  for (int i = 0; i < deg; i++) {
    min->children.remove(curr, false);
    curr->par = NULL;
    
    FibNode<K, V>* next = curr->right;
    roots_.insert(curr);
    
    curr = next;
  }
  
  size_--;
  
  if (size_ == 0) {
    min_root_ = NULL;
  }
  
  // Consolidate the heap and update minimum value node.
  consolidate();
  updateMin();
  
  K key = min->key;
  V value = min->value;
  
  map_.erase(key);
  delete min;
  return {key, value};
}

template<typename K, typename V>
void FibHeap<K, V>::decrease(const K& key, const V& newValue) {
  auto it = map_.find(key);
  if (it == map_.end()) return;
  
  FibNode<K, V>* node = it->second;
  if (node->value <= newValue) return;
  
  node->value = newValue;
  FibNode<K, V>* par = node->par;
  if (par != NULL && node->value < par->value) {
    // Perform a cut and possibly cascading cut.
    cut(par, node);
    cascadingCut(par);
  }
  
  updateMin();
}

template<typename K, typename V>
std::pair<K, V> FibHeap<K, V>::remove(const K& key) {
  auto it = map_.find(key);
  if (it == map_.end()) {
    throw std::invalid_argument(std::to_string(key) + " does not exist in heap!");
  }
  
  // Decrease key to cost -infinty, and extract it.
  std::pair<K, V> pr = {key, it->second->value};
  decrease(key, kNegInf);
  extractMin();
  
  return pr;
}

template<typename K, typename V>
V& FibHeap<K, V>::getValue(const K& key) const {
  auto it = map_.find(key);
  if (it == map_.end()) {
    throw std::invalid_argument(std::to_string(key) + " does not exist in heap!");
  }
  return it->second->value;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const FibHeap<K, V>& fh) {
  if (fh.roots_.empty()) {
    os << "Empty";
    return os;
  }
  
  std::stack<FibNode<K, V>*> nodes;
  FibNode<K, V>* curr = fh.roots_.node()->left;
  for (int i = 0; i < fh.roots_.size(); i++) {
    nodes.push(curr);
    curr = curr->left;
  }
  
  while (!nodes.empty()) {
    curr = nodes.top(); nodes.pop();
    os << "(" << curr->key << ", " << curr->value
       << "; " << curr->degree() << "; ";
    if (curr->par) {
      os << curr->par->key;
    } else {
      os << "--";
    }
    os << ") ";
    
    if (curr->children.empty()) {
      continue;
    }
    
    FibNode<K, V>* temp = curr->children.node()->left;
    for (int i = 0; i < curr->children.size(); i++) {
      nodes.push(temp);
      temp = temp->left;
    }
  }
  
  return os;
}

// MARK: - Private functions

template<typename K, typename V>
void FibHeap<K, V>::updateMin() {
  FibNode<K, V>* curr = roots_.node();
  min_root_ = curr;
  for (int i = 0; i < roots_.size(); i++) {
    if (curr->value < min_root_->value) {
      min_root_ = curr;
    }
    curr = curr->right;
  }
}

template<typename K, typename V>
void FibHeap<K, V>::consolidate() {
  std::vector<FibNode<K, V>*> A(size_);
  FibNode<K, V>* curr = roots_.node();
  
  // Consolidate all nodes with the same degree.
  int num_roots = roots_.size();
  while (num_roots > 0) {
    int deg = curr->degree();
    while (A[deg] != NULL) {
      // The other node with the same degree as curr.
      FibNode<K, V>* other = A[deg];
      if (other->value < curr->value) {
        // Swap the keys in place so we don't lose our position in the iterator.
        // Then attach larger node as child to smaller node.
        roots_.swap(other, curr);
        link(other, curr);
        curr = other;
      } else {
        link(curr, other);
      }
      A[deg] = NULL;
      deg++;
    }
    
    // "curr" now has degree deg.
    A[deg] = curr;
    
    curr = curr->right;
    num_roots--;
  }
}

// Assuming "child" and "par" were root nodes, adds "child" to child list of "par",
// removes "child" from root list, and sets parent of "child" to "par".
template<typename K, typename V>
void FibHeap<K, V>::link(FibNode<K, V>* par, FibNode<K, V>* child) {
  // Remove from roots list and capture ptr.
  roots_.remove(child, false);
  par->children.insert(child);
  child->par = par;
  child->mark = false;
}

// Remove "child" from the child list of "par" and add "child" to the
// root list.
template<typename K, typename V>
void FibHeap<K, V>::cut(FibNode<K, V>* par, FibNode<K, V>* child) {
  par->children.remove(child, false);
  roots_.insert(child);
  child->par = NULL;
  child->mark = false;
}

template<typename K, typename V>
void FibHeap<K, V>::cascadingCut(FibNode<K, V>* node) {
  FibNode<K, V>* par = node->par;
  if (par == NULL) return;
  
  if (!node->mark) {
    node->mark = true;
  } else {
    cut(par, node);
    cascadingCut(par);
  }
}

#endif /* fib_heap_h */

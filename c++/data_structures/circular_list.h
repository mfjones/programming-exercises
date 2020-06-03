//
//  circular_list.h
//  data_structures_xc
//
//  Created by Mitchell Jones on 6/2/20.
//  Copyright © 2020 Mitchell Jones. All rights reserved.
//

#ifndef circular_list_h
#define circular_list_h

#include <iostream>
#include <vector>

template<typename V>
struct SimpleNode {
  V value;
  SimpleNode<V>* left = NULL;
  SimpleNode<V>* right = NULL;
};

template<typename V, typename N = SimpleNode<V>>
class CircularList {
  N* node_;
  int size_;
  
public:
  CircularList() : size_(0), node_(NULL) {};
  ~CircularList();
  void insert(V value);
  void insert(N* node);
  void insertAfter(N* after_node, V value);
  void insertAfter(N* after_node, N* node);
  void remove(V value, bool free = true);
  void remove(N* node, bool free = true);
  N* unlink(N* node);
  void swap(N* n1, N* n2);
  N* find(V value);
  
  // Returns a pointer to an *arbitrary* node in the circular linked list.
  N* node() const { return node_; };
  int size() const { return size_; }
  bool empty() const { return size_ == 0; };
  
  template<typename U, typename S>
  friend std::ostream& operator<<(std::ostream& os, const CircularList<U, S>& cl);
};

template<typename V, typename N>
CircularList<V, N>::~CircularList() {
  while (size_ > 0) {
    remove(node());
  }
}

template<typename V, typename N>
void CircularList<V, N>::insert(V value) {
  // Is the list empty?
  if (empty()) {
    N* new_node = new N;
    new_node->value = value;
    insert(new_node);
    return;
  }
  
  insertAfter(node_, value);
}

template<typename V, typename N>
void CircularList<V, N>::insert(N* node) {
  if (empty()) {
    size_++;
    node->left = node;
    node->right = node;
    node_ = node;
    return;
  }
  
  insertAfter(node_, node);
}

template<typename V, typename N>
void CircularList<V, N>::insertAfter(N* after_node, V value) {
  N* new_node = new N;
  new_node->value = value;
  insertAfter(after_node, new_node);
}

template<typename V, typename N>
void CircularList<V, N>::insertAfter(N* after_node, N* node) {
  N* next = after_node->right;
  after_node->right = node;
  node->left = after_node;
  node->right = next;
  next->left = node;
  
  size_++;
}

template<typename V, typename N>
void CircularList<V, N>::remove(V value, bool free) {
  remove(find(value), free);
}

// Removes an element from the list. If "free" is set to false, the
// objected pointed to by node is *not* released. YOU are responsible
// for freeing it later on.
template<typename V, typename N>
void CircularList<V, N>::remove(N* node, bool free) {
  if (node == NULL) return;
  
  if (size() == 1) {
    if (free) delete node;
    
    node_ = NULL;
    size_ = 0;
    return;
  }
  
  N* left = node->left;
  N* right = node->right;
  
  left->right = right;
  right->left = left;
  
  size_--;
  
  if (node_ == node) {
    node_ = node->right;
  }
  
  if (free) delete node;
  
  return;
}

// Swaps located of two items in the circular list.
// See https://stackoverflow.com/a/27041055/1702220 for explanation.
template<typename V, typename N>
void CircularList<V, N>::swap(N* A, N* B) {
  if (A == B) return;
  
  // No need to swap in a circular linked list of size two.
  if (size_ == 2) return;
  
  std::vector<N*> swap_vec(4);
  N* temp;
  
  // If A and B are adjacent, assume A comes before B. Swap the pointers.
  if (B->right == A) {
      temp = A;
      A = B;
      B = temp;
  }

  swap_vec[0] = A->left;
  swap_vec[1] = B->left;
  swap_vec[2] = A->right;
  swap_vec[3] = B->right;

  // Test if A and B are neighbors.
  if ((A->right == B && B->left == A) || (A->right == B && B->left == A)) {
    A->left = swap_vec[2];
    B->left = swap_vec[0];
    A->right = swap_vec[3];
    B->right = swap_vec[1];
  } else {
    A->left = swap_vec[1];
    B->left = swap_vec[0];
    A->right = swap_vec[3];
    B->right = swap_vec[2];
  }
  
  // Reset the outer pointers.
  A->left->right = A;
  A->right->left = A;
  B->left->right = B;
  B->right->left = B;
}

// Find a value in O(n) time.
template<typename V, typename N>
N* CircularList<V, N>::find(V value) {
  N* curr = node_;
  for (int i = 0; i < size_; i++) {
    if (curr->value == value) {
      return curr;
    }
    curr = curr->right;
  }
  return NULL;
}

template<typename V, typename N>
std::ostream& operator<<(std::ostream& os, const CircularList<V, N>& cl) {
  N* n = cl.node_;
  os << "Starting with min value (size = " << cl.size_ << "): ";
  if (cl.empty()) {
    os << "Empty!";
    return os;
  }
  
  for (int i = 0; i < cl.size_-1; i++) {
    os << n->value << ", ";
    n = n->right;
  }
  
  os << n->value;
  return os;
}

#endif /* circular_list_h */

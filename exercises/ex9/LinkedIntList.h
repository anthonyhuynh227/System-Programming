// Copyright 2022 Isabella Bunnell<ibunnell@uw.edu>
// Phuoc Huynh<phuynh08@uw.edu>
#ifndef LINKEDINTLIST_H_
#define LINKEDINTLIST_H_

#include <memory>
#include "IntList.h"
using std::shared_ptr;
using std::weak_ptr;

namespace intlist333 {

// LinkedIntList is an implementation of
// the IntList abstract class. Implemented using
// a doubly linked list
class LinkedIntList : public IntList {
 public:
  LinkedIntList();
  // Destructor should be kept default
  // since smart pointers should clean
  // everything up for us
  virtual ~LinkedIntList() = default;
  // Returns the number of elements in the LinkedIntList
  int num_elements() const override;

  // Pushes the given integer on to the front of the List
  void Push(const int payload) override;

  // Pushes the given integer on to the back of the List
  void Append(const int payload) override;

  // Removes the first element in the List and returns it through payload_ptr.
  // Returns true if removal is successful and false if an element cannot be
  // removed.
  bool Pop(int* const payload_ptr) override;

  // removes the last element in the List
  // that element is returned through payload_ptr
  // returns true if removal is successful
  // false if an element cannot be removed
  bool Slice(int* const payload_ptr) override;

 private:
  int num_elements_;
  // Note that these should be modified
  // to use smart pointers at some point
  struct Node {
    Node(int* val, Node* nextNode, Node* prevNode):
                                          payload(shared_ptr<int>(val)),
                                          next(shared_ptr<Node>(nextNode)),
                                          prev(shared_ptr<Node>(prevNode)) {}
    ~Node() {}
    shared_ptr<int> payload;
    shared_ptr<Node> next;
    weak_ptr<Node> prev;
  };
  shared_ptr<Node> head_;
  shared_ptr<Node> tail_;
};

}  // namespace intlist333

#endif  // LINKEDINTLIST_H_

// Copyright 2022 Isabella Bunnell<ibunnell@uw.edu>
// Phuoc Huynh<phuynh08@uw.edu>
#include "LinkedIntList.h"
#include <iostream>

namespace intlist333 {
    LinkedIntList::LinkedIntList() {
        num_elements_ = 0;
        head_ = NULL;
        tail_ = NULL;
    }

    int LinkedIntList::num_elements() const {
        return num_elements_;
    }

    void LinkedIntList::Push(const int payload) {
        shared_ptr<Node> curr = shared_ptr<Node>
                        (new Node(new int(payload), nullptr, nullptr));
        if (num_elements_ == 0) {
            head_ = tail_ = curr;
            num_elements_ = 1;
        } else {
            shared_ptr<Node> head = head_;
            head->prev = curr;
            curr->next = head;
            head_ = curr;
            num_elements_ += 1;
        }
    }

    void LinkedIntList::Append(const int payload) {
        shared_ptr<Node> curr = shared_ptr<Node>
                        (new Node(new int(payload), nullptr, nullptr));
        if (num_elements_ == 0) {
            head_ = tail_ = curr;
            num_elements_ = 1;
        } else {
            shared_ptr<Node> tail = tail_;
            tail->next = curr;
            curr->prev = tail;
            tail_ = curr;
            num_elements_ += 1;
        }
    }

    bool LinkedIntList::Pop(int *const payload_ptr) {
        if (num_elements_ == 0) {
            return false;
        }
        shared_ptr<Node> head = head_;
        *payload_ptr = *(head->payload);
        if (num_elements_ == 1) {
            head_ = nullptr;
            tail_ = nullptr;
            num_elements_ = 0;
            return true;
        } else {
            head_ = head->next;
            head_->prev.lock() = nullptr;
            num_elements_ -= 1;
            return true;
        }
    }

    bool LinkedIntList::Slice(int *const payload_ptr) {
        if (num_elements_ == 0) {
            return false;
        }
        shared_ptr<Node> tail = tail_;
        *payload_ptr = *(tail->payload);
        if (num_elements_ == 1) {
            head_ = nullptr;
            tail_ = nullptr;
            num_elements_ = 0;
            return true;
        } else {
            tail_ = tail->prev.lock();
            tail_->next = nullptr;
            num_elements_ -= 1;
            return true;
        }
    }
}  // namespace intlist333

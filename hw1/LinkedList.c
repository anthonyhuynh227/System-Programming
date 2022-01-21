/*
 * Copyright ©2022 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2022 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>
#include <stdlib.h>

#include "CSE333.h"
#include "LinkedList.h"
#include "LinkedList_priv.h"


///////////////////////////////////////////////////////////////////////////////
// LinkedList implementation.

LinkedList* LinkedList_Allocate(void) {
  // Allocate the linked list record.
  LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
  Verify333(ll != NULL);

  // STEP 1: initialize the newly allocated record structure.
  ll->num_elements = 0;
  ll->head = NULL;
  ll->tail = NULL;

  // Return our newly minted linked list.
  return ll;
}

void LinkedList_Free(LinkedList *list,
                     LLPayloadFreeFnPtr payload_free_function) {
  Verify333(list != NULL);
  Verify333(payload_free_function != NULL);

  // STEP 2: sweep through the list and free all of the nodes' payloads
  // (using the payload_free_function supplied as an argument) and
  // the nodes themselves.

  // iterate thru LL until LL is finished
  while (list->head != NULL) {
    // first we free the payload that the LL head points to
    payload_free_function(list->head->payload);
    // then we will free the LL Head itself
    // first we save the LL Node Pointer so we can move to the next pointer
    // and save this one before freeing it
    LinkedListNode* destroyedHead = list->head;
    list->head = list->head->next;
    free(destroyedHead);
  }

  // free the LinkedList
  free(list);
}

int LinkedList_NumElements(LinkedList *list) {
  Verify333(list != NULL);
  return list->num_elements;
}

void LinkedList_Push(LinkedList *list, LLPayload_t payload) {
  Verify333(list != NULL);

  // Allocate space for the new node.
  LinkedListNode *ln = (LinkedListNode *) malloc(sizeof(LinkedListNode));
  Verify333(ln != NULL);

  // Set the payload
  ln->payload = payload;

  if (list->num_elements == 0) {
    // Degenerate case; list is currently empty
    Verify333(list->head == NULL);
    Verify333(list->tail == NULL);
    ln->next = ln->prev = NULL;
    list->head = list->tail = ln;
    list->num_elements = 1;
  } else {
    // STEP 3: typical case; list has >=1 elements
    Verify333(list->head != NULL);
    Verify333(list->tail != NULL);

    // make sure that ln (the new head) doesn't have a val
    // ahead of it
    ln->prev = NULL;
    // old head of list is now the "next" element,
    // as ln becomes the new head
    LinkedListNode* oldHead = list->head;
    ln->next = oldHead;
    // set ln to front of LL
    oldHead->prev = ln;
    // update entire list metadata
    list->num_elements += 1;
    list->head = ln;
  }
}

bool LinkedList_Pop(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  // STEP 4: implement LinkedList_Pop.  Make sure you test for
  // and empty list and fail.  If the list is non-empty, there
  // are two cases to consider: (a) a list with a single element in it
  // and (b) the general case of a list with >=2 elements in it.
  // Be sure to call free() to deallocate the memory that was
  // previously allocated by LinkedList_Push().

  // testing for empty case and failing.
  if (list->num_elements == 0) {
    return false;
  }

  // save previous head before we do anything to it.
  LinkedListNode* oldHead = list->head;

  if (list->num_elements == 1) {
    // Degenerate case; list currently only has one node
    // set list metadata to be 0 for everything
    list->head = NULL;
    list->tail = NULL;
    list->num_elements = 0;
  } else {
    // typical case; list has >1 elements
    // remove current head from metadata (make next element the new head)
    list->head = list->head->next;
    // update new head's prev value (since it now the front)
    list->head->prev = NULL;
    list->num_elements -= 1;
  }

  // put old head payload into payload pointer
  // (dereference)
  *payload_ptr = oldHead->payload;
  // free old head
  free(oldHead);

  // success
  return true;
}

void LinkedList_Append(LinkedList *list, LLPayload_t payload) {
  Verify333(list != NULL);

  // STEP 5: implement LinkedList_Append.  It's kind of like
  // LinkedList_Push, but obviously you need to add to the end
  // instead of the beginning.
  // allocate space for new node to be added to the end of list
  LinkedListNode* new_node = (LinkedListNode*) malloc(sizeof(LinkedListNode));

  // if space for a node, set the payload of the new node
  new_node->payload = payload;

  if (list->num_elements == 0) {
    // Degenerate case; list is currently empty
    // this is basically taken from Push, because in this case
    // theyre the same thing
    Verify333(list->head == NULL);
    Verify333(list->tail == NULL);
    new_node->next = new_node->prev = NULL;
    list->head = list->tail = new_node;
    list->num_elements = 1;
  } else {
    // typical case; list has >= 1 elements
    // this is basically taken from Push except on tail instead
    // of head
    Verify333(list->head != NULL);
    Verify333(list->tail != NULL);

    // make sure that new_node (the new tail) doesn't have a val
    // after it
    new_node->next = NULL;
    // old tail of list is now the "prev" element to
    // the new tail.
    LinkedListNode* oldTail = list->tail;
    new_node->prev = oldTail;
    // update next pointer of oldTail
    oldTail->next = new_node;
    // update entire list metadata
    list->num_elements += 1;
    list->tail = new_node;
  }
}

void LinkedList_Sort(LinkedList *list, bool ascending,
                     LLPayloadComparatorFnPtr comparator_function) {
  Verify333(list != NULL);
  if (list->num_elements < 2) {
    // No sorting needed.
    return;
  }

  // We'll implement bubblesort! Nnice and easy, and nice and slow :)
  int swapped;
  do {
    LinkedListNode *curnode;

    swapped = 0;
    curnode = list->head;
    while (curnode->next != NULL) {
      int compare_result = comparator_function(curnode->payload,
                                               curnode->next->payload);
      if (ascending) {
        compare_result *= -1;
      }
      if (compare_result < 0) {
        // Bubble-swap the payloads.
        LLPayload_t tmp;
        tmp = curnode->payload;
        curnode->payload = curnode->next->payload;
        curnode->next->payload = tmp;
        swapped = 1;
      }
      curnode = curnode->next;
    }
  } while (swapped);
}


///////////////////////////////////////////////////////////////////////////////
// LLIterator implementation.

LLIterator* LLIterator_Allocate(LinkedList *list) {
  Verify333(list != NULL);

  // OK, let's manufacture an iterator.
  LLIterator *li = (LLIterator *) malloc(sizeof(LLIterator));
  Verify333(li != NULL);

  // Set up the iterator.
  li->list = list;
  li->node = list->head;

  return li;
}

void LLIterator_Free(LLIterator *iter) {
  Verify333(iter != NULL);
  free(iter);
}

bool LLIterator_IsValid(LLIterator *iter) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);

  return (iter->node != NULL);
}

bool LLIterator_HasNext(LLIterator *iter) {
  if (LLIterator_IsValid(iter)) {
    // we know that there is a valid iterator, so now check next element
    if (iter->node->next == NULL) {
      return false;
    }
    return true;
  }
  return false;
}

bool LLIterator_Next(LLIterator *iter) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  // STEP 6: try to advance iterator to the next node and return true if
  // you succeed, false otherwise
  // Note that if the iterator is already at the last node,
  // you should move the iterator past the end of the list

  if (LLIterator_HasNext(iter)) {
    iter->node = iter->node->next;
    return true;
  }

  // move iterator past end of list
  iter->node = NULL;
  return false;
}

void LLIterator_Get(LLIterator *iter, LLPayload_t *payload) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  *payload = iter->node->payload;
}

bool LLIterator_Remove(LLIterator *iter,
                       LLPayloadFreeFnPtr payload_free_function) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);
  Verify333(payload_free_function != NULL);

  // STEP 7: implement LLIterator_Remove.  This is the most
  // complex function you'll build.  There are several cases
  // to consider:
  // - degenerate case: the list becomes empty after deleting.
  // - degenerate case: iter points at head
  // - degenerate case: iter points at tail
  // - fully general case: iter points in the middle of a list,
  //                       and you have to "splice".
  //
  // Be sure to call the payload_free_function to free the payload
  // the iterator is pointing to, and also free any LinkedList
  // data structure element as appropriate.


  // free payload that iterator is pointing to
  payload_free_function(iter->node->payload);
  // keep pointer to node that needs to be removed
  LinkedListNode* removed_node = iter->node;

  // DEGENERATE CASE 1: list becomes empty after deleting
    // - if there was only one element in the list, the iterator is now invalid
    //   and cannot be used.  In this case, the caller is recommended to free
    //   the now-invalid iterator.

  if (iter->list->num_elements == 1) {
    iter->node = NULL;
    // set all list metadata to 0
    iter->list->num_elements = 0;
    iter->list->head = NULL;
    iter->list->tail = NULL;

    // free node
    free(removed_node);
    // function is meant to return
    // false if the deletion succeeded, but the list is now empty.
    return false;
  }
  // DEGENERATE CASE 2: iter points at head
    // - if the deleted node had a successor (ie, it was pointing at the head),
    //   the iterator is now pointing at the successor.
  if (iter->node->prev == NULL) {
    iter->node = removed_node->next;
    iter->node->prev = NULL;
    // update list metadata
    iter->list->head = iter->node;
  } else if (iter->node->next == NULL) {
    // DEGENERATE CASE 3: iter points at tail
    // - if the deleted node was the tail, the iterator is now pointing at the
    //   predecessor.
    iter->node = removed_node->prev;
    iter->node->next = NULL;
    // update list metadata
    iter->list->tail = iter->node;
  } else {
    // FULLY GENERAL CASE: iter points in the middle of a list,
    //                     and you have to "splice".
    // basically want to "skip" this current node. prev and next nodes
    // on either side of removed node should be pointing to each other.
    removed_node->next->prev = removed_node->prev;
    removed_node->prev->next = removed_node->next;
    // then iterate to next node
    iter->node = removed_node->next;
  }

  // update list metadata size.
  iter->list->num_elements -= 1;
  // free node to be deleted.
  free(removed_node);
  // true if the deletion succeeded, and the list is still non-empty.
  return true;
}


///////////////////////////////////////////////////////////////////////////////
// Helper functions

bool LinkedList_Slice(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  // STEP 8: implement LinkedList_Slice.

  // since Slice is the "tail" version of Pop,
  // this is largely taken from Pop but for the tail.
  // testing for empty case and failing.
  if (list->num_elements == 0) {
    return false;
  }

  // save previous tail before we do anything to it.
  LinkedListNode* oldTail = list->tail;

  if (list->num_elements == 1) {
    // Degenerate case; list currently only has one node
    // set list metadata to be 0 for everything
    list->head = NULL;
    list->tail = NULL;
    list->num_elements = 0;
  } else {
    // typical case; list has >1 elements
    // remove current tail from metadata (make prev element the new tail)
    list->tail = list->tail->prev;
    // update new tails' next value (since it now the end)
    list->tail->next = NULL;
    list->num_elements -= 1;
  }

  // put old tail payload into payload pointer where value is.
  // (dereference)
  *payload_ptr = oldTail->payload;
  // free old tail
  free(oldTail);

  // success
  return true;
}

void LLIterator_Rewind(LLIterator *iter) {
  iter->node = iter->list->head;
}

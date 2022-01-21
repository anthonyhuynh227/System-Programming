# Bug 1

## A) How is your program acting differently than you expect it to?
- When running example_program_ht.c, we found that the program was failing at line 93, where the key was equaling 100 (which was previously deleted from the HashTable, verified by line 83)

line 83: Verify333(!HashTable_Find(ht, (HTKey_t)100, &kv));
line 93: Verify333(kv.key != (HTKey_t)100);


## B) Brainstorm a few possible causes of the bug
- For some reason, a reference to the key 100 was being accessed somewhere in the iterator even though it wasn't in the HT.
- When running HTIterator_Get(line 92), the key-value pair actually was not updating (from before the while loop on line 91).
- When running HTIterator_Get(line 92), the key-value pair actually was updating, but updating incorrectly and returning a value that it wasn't meant to.

## C) How you fixed the bug and why the fix was necessary
- It turns out from running the program through GDB (after making the values that were being tested smaller so the while loop would not have to run 100 times to get to the bad value) that it was failing on the first run of the while-loop because the key-value pair was not being updating, it was still stuck on the last value that was removed. To fix this, we had to look at our HTIterator_Get function and it turned out that we were passing in the wrong pointer to be updated by Get within the function. Line 383 of HashTable.c was putting the return at (LLPayload_t)&keyvalue, but it tneeded to be (LLPayload_t)keyvalue instead.


# Bug 2

## A) How is your program acting differently than you expect it to?
- After we fixed Bug 1, we ran into another error where the program was saying:

free() double free detected in tcache 2
Aborted (core dumped)

## B) Brainstorm a few possible causes of the bug
Because we knew that the program had not been failing after the point of line 93, we knew that the bug could lie with any Free function called after line 93. This nailed it down to the following functions:
- Double free happening within HTIterator_Free();
- Double free happening within HashTable_Free();
- Double free happening within HTIterator_Next();

## C) How you fixed the bug and why the fix was necessary
- We put a few print statements down to see what was being executed before the double free, and it turned out that it was happening during the function HTIterator_Next(). There were a few places we were freeing LLIterator within that function because there were many cases that we needed to handle. It turned out that we did not handle these case completely correctly the first time because we were calling the LinkedList iterator free function within a while loop to iterate through the number of buckets when we should only have freed the linked list the first time (before the while loop executed) to free the Linked List that the iterator had just passed. When we were doing it within the while loop, we were freeing LLIterators that had not existed yet!


# Bug 3

## A) How is your program acting differently than you expect it to?
- We got a memory leak of 192 bytes allocated in test_hashtable.cc, line 149. The valgrind trace showed that the problem was happening within HashTable_Remove --> KeyExistsInChain --> LLIterator_Allocate.

## B) Brainstorm a few possible causes of the bug
There are a few things that could not have been free's properly within our helper function KeyExistsInChain.
- We allocated space for an iterator withing KeyExistsInChain, so in one of our cases we may have been forgetting to free the LLIterator before we returned the function.
- Since this was happening withing HashTable_Remove, it could be that the reference to the LL node was being removed from the LL but the node itself was not freed.
- Similar thing could have been happening where maybe the LL node was being freed, but the payload itself was not freed.

## C) How you fixed the bug and why the fix was necessary
- When we looked at KeyExistsInChain, and since we knew the issue was happening within HashTable_Remove, we looked at what parameters were being passed into HashTable_Remove. It was the case where swap false but delete was true. We were calling LLIterator_Remove to remove the current node in the chain, but the payload_free_function that was being passed in was LLNoOpFree, which wasn't actually freeing the payload (mixed up freeing the node itself vs freeing the payload, as they are two different things but very similar). So what we had to do was create another helper function called payload_Free that would actually free the payload. This fixed the memory leak.

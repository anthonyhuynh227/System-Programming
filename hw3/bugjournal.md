# Bug 1

## A) How is your program acting differently than you expect it to?
- We were getting a segfault in LLIterator_Get() that was called from our WriteHTBucket function in WriteIndex. When we ran the backtrace it was specifically saying that *payload = iter->node->payload in the LinkedList was causing the segfault.

## B) Brainstorm a few possible causes of the bug
We knew that segfaults generally happen when we are trying to access something that is currently NULL. So we had to look at things that may have been NULL, such as...
- the iterator itself was null (unlikely since it pointed to the payload part where the segfault was)
- the payload pointer was null
- the node trying to be accessed in the iterator was null

## C) How you fixed the bug and why the fix was necessary
- It turned out that the payload pointer we gave the LLIterator_Get() function was unitialised, because we had declared a HTKeyValue_t* kv and then passed whis into LLIterator_Get(). Since this pointer was uninitialized it was putting the contents to somewhere we didn't know. To fix, we instead declared a HTKeyValue_t kv and then passed in &kv into the LLIterator_Get().


# Bug 2

## A) How is your program acting differently than you expect it to?
- The checksum that was being returned by our WriteIndex function was not matching the expected checksum in the WriteIndex tests.

## B) Brainstorm a few possible causes of the bug
- The way we were calculating the checksum was wrong; perhaps we had forgotten to convert the final checksum
- The contents that made up the checksum itself, specifically the doctable was not being written correctly.
- The contents that made up the checksum itself, specifically the index was not being written correctly.

## C) How you fixed the bug and why the fix was necessary
- To eliminate the first possibility, we just really had to check that our procedure was sound within WriteHeader. It was, so we knew we had to look at how either the doctable or index was being written. To solve this bug we just had to go through each function and review the procedure and it turned out that in the index writer function we had forgotten to fseek to the position within the file before we wrote to it! So once we called fseek, it ended up working.


# Bug 3

## A) How is your program acting differently than you expect it to?
- We found that with the QueryProcessor tests, the correct documents were being returned but the rank was not correct (either it wasn't matching the expected value or it wasn't being returned in the correct order of ranks as expected).

## B) Brainstorm a few possible causes of the bug
- The ranks being returned by the header were not correct.
- The way the ranks were being calculated for each document was not correct.
- We maybe were not adding all the headers that had the word to our list (like we were leaving some out from our logic to filter all documents that contained all of the words in the query) so that's why the rank was not correct.

## C) How you fixed the bug and why the fix was necessary
- It turned out that the way the ranks were being calculated was not correct, it's not that the logic was wrong (as it really was just addition) but rather as we iterated through the list of QueryResults in our final QueryResult list to find the QueryResult that should be updated, the for-each loop signature was (QueryResult res : final_result), so when we tried to update res.rank this was only a reference to res.rank not the res.rank itself. It needed to be for (QueryResult &res : final_result) so that we had the actual address of the QueryResult. We found this out by comparing the list of headers, their docID and rank to the list final list of query results at the end.

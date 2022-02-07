# Bug 1

## A) How is your program acting differently than you expect it to?
- When running test_suite, we found that the program was failing at line 111 in test_crawlfiletree.cc, where the the number of mappings inside the DocTable should be 31 insteads of 2. 
Line 111: ASSERT_EQ(31, DocTable_NumDocs(doc_table)); 

## B) Brainstorm a few possible causes of the bug
- Something was wrong with our DocTable_Add method, perhaps the number
of docs were not getting updated properly inside there.
- In our CrawlFileTree, the directories may not be getting counted properly.
- To test this, we had originally put a bunch of Verify333 statements within the DocTable_Add and realised that sometimes the HashTable inserts were failing. It possibly could have been an uncaught bug (as we were using our HashTable from HW1) but this was unlikely since the tests seemed pretty thorough.

## C) How you fixed the bug and why the fix was necessary
- It turns out the problem was the second one in HandleDir, when the iteration index i was being incremented, we forgot to decrement i again to account for invalid directories since i is meant to count how many entries we have. This was giving inconsistent results with our DocTable.


# Bug 2

## A) How is your program acting differently than you expect it to?
- When running Valgrind, we got 82 errors from 1 context. Valgrind showed
that we got invalid read memory at line 250 from FileParser.c 
if(isalpha(*(cur_ptr - 1)) && cur_ptr != content))
## B) Brainstorm a few possible causes of the bug
- cur_ptr might have been NULL, and we forgot to check for that case before
line 250 ran.
- When we decrement the pointer it was not correct, it was accessing invalid memory.
- calling isalpha() on invalid memory may have caused the read error

## C) How you fixed the bug and why the fix was necessary
- When we look at given code of InsertContent function, we realized that
cur_ptr was initialized equal to content char pointer. We found out that
cur_ptr - 1 could lead to invalid memory in the case cur_ptr at the begin of content. We put another condition in our if statement to check that when we decrement the current pointer it is always bigger than content, therefore valid.

# Bug 3

## A) How is your program acting differently than you expect it to?
- When running tests for searchshell (on the test_tree/tiny directory), when trying to enter in queries we were getting a segmentation fault. 
## B) Brainstorm a few possible causes of the bug
- We knew that segfaults would happen if something that was NULL was trying to be accessed. So we had to look at places where NULL memory might occur. Perhaps when trying to convert all characters to lowercase from the user input, the user input was NULL.
- Perhaps after the lowercase conversion was not working correctly, causing line to point to NULL memory when trying to then take the user input and convert the entire line into tokens (left_to_read became NULL at some point).
- Wasn't checking to see if LinkedList iterator was NULL at first, perhaps this could have been the issue.

## C) How you fixed the bug and why the fix was necessary
-  It turns out that when converting the user input line to lowercase, we were checking the wrong array that would end the loop when the terminator '\0' was reached. We found this out by putting a bunch of printf statements everywhere and then running the searchshell to see what exactly would print out before the program segfaulted. When we printed out what the string was right before the loop to convert to lower, we found out that it was NULL, hence segfault. So to fix, we changed the array that was being read from 'tokens' (which had just been malloc'd but not used yet) to 'line'.
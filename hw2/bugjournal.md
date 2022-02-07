# Bug 1

## A) How is your program acting differently than you expect it to?
- When running test_suite, we found that the program was failing at line 111
in test_crawlfiletree.cc, where the the number of mappings inside the DocTable
should be 31 insteads of 2. 
Line 111: ASSERT_EQ(31, DocTable_NumDocs(doc_table)); 

## B) Brainstorm a few possible causes of the bug
- 
- 
- 

## C) How you fixed the bug and why the fix was necessary
- 


# Bug 2

## A) How is your program acting differently than you expect it to?
- When running test_crawlfiletree.cc, we found that program was failing at 
line 120, where the doc_name we get from DocTable_GetDocName based on DocId
is not the same as expected from actual name which is dir_path.
Line 120: ASSERT_STREQ(doc_name, dir_path);
## B) Brainstorm a few possible causes of the bug
- 
- 
- 

## C) How you fixed the bug and why the fix was necessary
- 


# Bug 3

## A) How is your program acting differently than you expect it to?
- When running Valgrind, we got 82 errors from 1 context. Valgrind showed
that we got invalid read memory at line 250 from FileParser.c 
if(isalpha(*(cur_ptr - 1)) && cur_ptr != content))
## B) Brainstorm a few possible causes of the bug
- 
- 
- 

## C) How you fixed the bug and why the fix was necessary
- When we look at given code of InsertContent function, we realizedthat
cur_ptr was initialized equal to content char pointer. We found out that
cur_ptr - 1 could lead to inval memory in the case cur_ptr at the begin of
content. 

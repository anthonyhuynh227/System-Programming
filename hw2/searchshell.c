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

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
static void Usage(void);
static void ProcessQueries(DocTable* dt, MemIndex* mi, char** tokens, int query_len);
//static int GetNextLine(FILE* f, char** ret_str);


//////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char** argv) {
  if (argc != 2) {
    Usage();
  }

  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - Crawl from a directory provided by argv[1] to produce and index
  //  - Prompt the user for a query and read the query from stdin, in a loop
  //  - Split a query into words (check out strtok_r)
  //  - Process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.
  //
  // Note that you should make sure the fomatting of your
  // searchshell output exactly matches our solution binaries
  // to get full points on this part.

  // variable declarations
  DocTable* doc_table;
  MemIndex* index;
  // crawl from directory provided by argv[1]
  bool file_crawl_result = CrawlFileTree(argv[1], &doc_table, &index);

  if (file_crawl_result == false) {
    printf("Could not crawl directory given in argument!\n");
    Usage();
  }

  // prompt the user for a query and read the query from stdin, in a loop
  // EOF - ctrl+D: breaks loop
  while (true) {
    printf("please enter query:\n");
    char line[256];
    if (fgets(line, sizeof(line), stdin)) {
      // char* -> string
      // char** -> array of strings, same as char* query[]
      //printf("segfault at malloc tokens\n");
      char** tokens = (char**) malloc(sizeof(line) * sizeof(char*));
      // check for OOM error
      Verify333(tokens != NULL);
      //printf("segfault at converting chars to lowercase\n");

      // iterate through line, convert all chars to lowercase
      int i = 0;
      //printf("string is: %s\n", *tokens);
      while (line[i] != '\0') {
        //printf("segfault at converting %c token to lower\n", (char)tolower(line[i]));
        line[i] = (char)tolower(line[i]);
        i++;
      }

      // Split the query into words (check out strtok_r)
      char* left_to_read = line;
      char* token;
      int j = 0;
      //printf("segfault at converting tokens\n");
      while (true) {
        token = strtok_r(left_to_read, " ", &left_to_read);
        if (token == NULL || *token == '\n') {
          break; // reached the end of tokens
        }
        tokens[j] = token;
        j++;
      }

      int query_len = j;
      // replace newline (when press enter) to '\0'
      //printf("segfault at replacing new line\n");
      char* new_line = strchr(tokens[query_len - 1], '\n');
      if (new_line) {
        *new_line = '\0';
      }

      // Process a query against the index and print out the results
      ProcessQueries(doc_table, index, tokens, query_len);
      free(tokens);

    } else {
      printf("\ngoodbye!\n");
      break;
    }
  }
  MemIndex_Free(index);
  DocTable_Free(doc_table);
  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Helper function definitions

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative " \
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}

static void ProcessQueries(DocTable* dt, MemIndex* mi, char** tokens, int query_len) {
  //printf("segfault at searching memindex\n");
  LinkedList* results = MemIndex_Search(mi, tokens, query_len);

  if (results == NULL) { // no matches found
    return;
  }

  //printf("segfault at allocating iter\n");
  LLIterator* iter = LLIterator_Allocate(results);
  // print linked list results against doc table

  SearchResult* res;
  char* current_directory;
  //printf("segfault at looping through iter\n");
  while (LLIterator_IsValid(iter)) {
    LLIterator_Get(iter, (LLPayload_t*)&res);
    
    // now that we have doc_id, get directory name
    //printf("segfault at getting docname\n");
    current_directory = DocTable_GetDocName(dt, res->doc_id);
    printf("    directory: %s\n", current_directory);
    printf("        rank: %d\n", res->rank);

    LLIterator_Next(iter);
  }

  //printf("segfault at freeing stuff\n");
  LinkedList_Free(results, &free);
  LLIterator_Free(iter);
}

/*
static int GetNextLine(FILE* f, char** ret_str) {
  return -1;  // you may want to change this
}
*/


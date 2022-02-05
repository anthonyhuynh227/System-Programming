#define _XOPEN_SOURCE 600

#include "./FileParser.h"
#include "libhw1/LinkedList.h"
#include "libhw1/HashTable.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include "libhw1/CSE333.h"
#include "./MemIndex.h"

char* copy_string(char* src, int size) {
  char* str = (char *) malloc(sizeof(char) * size);
  int i;
  while (i < size) {
    *str = *src;
    str++;
    i++;
  }
  *str = '\0'; // don’t forget the null terminator!
  return str;
}

void AddWordPosition(HashTable* tab, char* word,
                    DocPositionOffset_t pos) {
  HTKey_t hash_key;
  HTKeyValue_t kv;
  WordPositions *wp;

  // Hash the string.
  hash_key = FNVHash64((unsigned char*) word, strlen(word));

  // Have we already encountered this word within this file?  If so, it's
  // already in the hashtable.
  if (HashTable_Find(tab, hash_key, &kv)) {
    // Yes; we just need to add a position in using LinkedList_Append(). Note
    // how we're casting the DocPositionOffset_t position variable to an
    // LLPayload_t to store it in the linked list payload without needing to
    // malloc space for it.  Ugly, but it works!
    wp = (WordPositions*) kv.value;

    // Ensure we don't have hash collisions (two different words that hash to
    // the same key, which is very unlikely).
    Verify333(strcmp(wp->word, word) == 0);

    LinkedList_Append(wp->positions, (LLPayload_t) (int64_t) pos);
  } else {
    // STEP 7.
    // No; this is the first time we've seen this word.  Allocate and prepare
    // a new WordPositions structure, and append the new position to its list
    // using a similar ugly hack as right above.
    WordPositions* new_node = (WordPositions*) malloc(sizeof(WordPositions));
    new_node->positions = LinkedList_Allocate();
    new_node->word = word;
    LinkedList_Append(new_node->positions, (LLPayload_t) (int64_t) pos);

    // add to hashtable
    kv.value = (void*) new_node;
    kv.key = hash_key;
    HashTable_Insert(tab, kv, NULL);
  }
}

void InsertContent(HashTable* tab, char* content) {
  char* cur_ptr = content;
  char* word_start = content;

  bool found_start_of_word = true;
  int word_size = 0;
  int index = 0;
  while (*cur_ptr != '\0') {
    if (isalpha(*cur_ptr)) { // reached the end of the word
      if (!found_start_of_word) {
        word_start = cur_ptr;
        found_start_of_word = true;
        word_size = 0;
      }
      *cur_ptr = tolower(*cur_ptr);
      cur_ptr += 1;
      word_size += 1;
    } else if (found_start_of_word) {
      // reached the end of a word, and have a full word (since we know word start)
      found_start_of_word = false;
      char* word = copy_string(word_start, word_size);
      // add word to hashmap
      AddWordPosition(tab, word, index - word_size);
    }
    index += 1;
  }  // end while-loop
}


int main() {
    HTKeyValue_t kv, old_kv;

    char* content = "The  Fox  Can't   CATCH the  Chicken.";
    HashTable* ht = HashTable_Allocate(10);
    InsertContent(ht, content);

    if (!HashTable_Find(ht, (HTKey_t) "the", &kv)) {
        printf("could not find 'the'\n");
    }

    if (!HashTable_Find(ht, (HTKey_t) "fox", &kv)) {
        printf("could not find 'fox'\n");
    }

    if (!HashTable_Find(ht, (HTKey_t) "can", &kv)) {
        printf("could not find 'can'\n");
    }

    if (!HashTable_Find(ht, (HTKey_t) "t", &kv)) {
        printf("could not find 't'\n");
    }

    if (!HashTable_Find(ht, (HTKey_t) "catch", &kv)) {
        printf("could not find 'catch'\n");
    }

    if (!HashTable_Find(ht, (HTKey_t) "chicken", &kv)) {
        printf("could not find 'chicken'\n");
    }
}
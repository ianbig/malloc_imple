#include <stdio.h>
#include <stdlib.h>

#include "my_malloc.h"

// Note: this is a memory leak program since it is for testing

int main() {
  init_memory_control_block();
  // remove from tail
  printf("========= Testing Remove Tail =========\n");
  memory_block_meta * ptrHead = malloc(sizeof(*ptrHead));
  insertToList(ptrHead);
  insertToList(malloc(sizeof(*ptrHead)));
  memory_block_meta * ptr = NULL;
  ptr = insertToList(malloc(sizeof(*ptr)));
  free(removeFromList(ptr));

  // remove head
  printf("======= Testing Remove Head =========\n");
  ptr = malloc(sizeof(*ptr));
  insertToList(ptr);
  free(removeFromList(ptrHead));

  printf("======= Testing Remove Middle =========\n");
  memory_block_meta * middle = malloc(sizeof(*ptr));
  insertToList(middle);
  insertToList(malloc(sizeof(*ptr)));
  insertToList(malloc(sizeof(*ptr)));
  insertToList(malloc(sizeof(*ptr)));
  free(removeFromList(ptr));

  return 0;
}

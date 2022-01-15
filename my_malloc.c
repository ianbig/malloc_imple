#include "my_malloc.h"

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

void * ff_malloc(size_t size) {
  if (block_manager == NULL) {
    init_memory_control_block();
  }
  // find free list for first freed block
}

void init_memory_control_block() {
  // initialized memory for block manager
  block_manager = sbrk(sizeof(memory_control_block));
  block_manager->freeListHead = NULL;

#ifdef NDEBUG
  fprintf(stderr, "block_manager at addr %p\n", block_manager);
#endif
}

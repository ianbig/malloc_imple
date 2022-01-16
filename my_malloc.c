#include "my_malloc.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// TODO: split the memory size
void * ff_malloc(size_t size) {
  if (block_manager == NULL) {
    init_memory_control_block();
  }
  // find free list for first freed block
  void * chunk = ff_getBlock(size);
  return chunk;
}

void ff_free(void * freeBlock) {
  if (block_manager == NULL) {
    init_memory_control_block();
  }

  if (freeBlock->type == MEM_FREE) {
    fprtinf(stderr, "Error: double free at adress %p\n", ptr);
    exit(EXIT_FAILURE);
  }

  freeBlock = mergeBlock(freeBlock);
  insertToList(startCurEnd);
}

void * insertToList(void * toAdd) {
  toAdd->nextBlock = block_manager->freeListHead;
  block->manager->freeListHead = toAdd;
}

void * mergeBlock(void * freeBlock) {
  void * curNode = block_manager->firstHead;
  void * freeBlockStart = freeBlock;
  void * freeBlockEnd = freeBlockStart + sizeof(memory_block_meta) + free_block->size;
  while (curNode != NULL) {
    void * startCurNode = curNode;
    void * endCurNode = startCurNode + sizeof(memory_block_meta) + curNode->size;
    if ((freeBlockStart - endCurNode == 1) || (startCurNode - freeBlockEnd == 1)) {
      curNode = removeFromList(curNode);
      if (freeBlockStart - endCurNode == 1) {
        freeBlock = curNode;
      }

      (memory_block_meta *)freeBlock->size += (memory_control_block *)curNode->size;
    }
  }

  retrun freeBlock;
}

void * ff_getBlock(size_t size) {
  memory_block_meta * freePtr = block_manager->freeListHead;
  while (freePtr != NULL) {
    if (freePtr->size >= size) {
      // size and startAddr can use previous info as it would not change
      // TODO: maintain the linked list
      freePtr->type = MEM_ALLOCATED;
      freePtr->nextBlock = NULL;
      return freePtr->data;
    }
  }

  // not enough space in free list
  memory_block_meta * newChunk = sbrk(size + sizeof(memory_block_meta));
  newChunk->size = size;
  newChunk->type = MEM_ALLOCATED;
  newChunk->nextBlock = NULL;
  newChunk->data = (void *)newChunk + sizeof(memory_block_meta);

#ifdef NDEBUG
  getBlock_info(newChunk);
#endif

  return newChunk->data;
}

void getBlock_info(memory_block_meta * chunk) {
  assert(chunk != NULL);
  fprintf(stderr, "=====printing info after ff_getBlock()=====\n");
  fprintf(stderr,
          "chunk %p, size: %zu, chunk type: %d, nextBlock: %p,  dataStartAddr: "
          "%p\n",
          chunk,
          chunk->size,
          chunk->type,
          chunk->nextBlock,
          chunk->data);
}

void init_memory_control_block() {
  // initialized memory for block manager
  block_manager = sbrk(sizeof(memory_control_block));
  block_manager->freeListHead = NULL;

#ifdef NDEBUG
  fprintf(
      stderr, "init_memory_control_block(): block_manager at addr %p\n", block_manager);
#endif
}

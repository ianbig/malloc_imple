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

void ff_free(void * toFree) {
  memory_block_meta * freeBlock = toFree - sizeof(memory_block_meta);
  assert(block_manager != NULL);

  if (freeBlock->type == MEM_FREE) {
    fprintf(stderr, "Error: double free at adress %p\n", freeBlock);
    exit(EXIT_FAILURE);
  }

  freeBlock->type = MEM_FREE;
  freeBlock = mergeBlock(freeBlock);
  insertToList(freeBlock);
}

void * insertToList(void * toAdd) {
  ((memory_block_meta *)toAdd)->nextBlock = block_manager->freeListHead;
  block_manager->freeListHead = toAdd;
#ifdef NDEBUG
  fprintf(stderr,
          "insertToList(): adding new chunk %p with size %zu to free list\n",
          toAdd,
          ((memory_block_meta *)toAdd)->size);
  printList();
#endif
}

// TODO: improve algorithmatic performance to O(1) by convert to doubly linked list
void * removeFromList(void * toRemove) {
  memory_block_meta ** ptr = (memory_block_meta **)(&(block_manager->freeListHead));
  while (*ptr != NULL && *ptr != toRemove) {
    ptr = &((*ptr)->nextBlock);
  }

  assert(*ptr != NULL);
  void * ret = *ptr;
  *ptr = (*ptr)->nextBlock;

#ifdef NDEBUG
  fprintf(stderr,
          "removeFromList(): remove chunk %p with size %zu\n",
          toRemove,
          ((memory_block_meta *)toRemove)->size);
  printList();
#endif
  return ret;
}

void printList() {
  memory_block_meta * ptr = block_manager->freeListHead;
  while (ptr != NULL) {
    fprintf(stderr, "%p with size %zu ---->  ", ptr, ptr->size);
    ptr = ptr->nextBlock;
  }

  fprintf(stderr, "NULL\n");
}

void * mergeBlock(memory_block_meta * freeBlock) {
  memory_block_meta * curNode = block_manager->freeListHead;

  while (curNode != NULL) {
    void * freeBlockStart = freeBlock;
    void * freeBlockEnd = freeBlockStart + sizeof(memory_block_meta) + freeBlock->size;
    void * startCurNode = curNode;
    void * endCurNode = startCurNode + sizeof(memory_block_meta) + curNode->size;

    if ((freeBlockStart == endCurNode) ||
        (startCurNode == freeBlockEnd)) {  // memory adjacent

      curNode = removeFromList(curNode);
      if (freeBlockStart == endCurNode) {
        void * tmp = curNode;
        curNode = freeBlock;
        freeBlock = tmp;
      }
      freeBlock->size += curNode->size + sizeof(*freeBlock);

#ifdef NDEBUG
      fprintf(
          stderr,
          "mergeBlock(): merging memory block %p with size %zu and %p with size %zu\n",
          freeBlock,
          freeBlock->size - curNode->size - sizeof(*freeBlock),
          curNode,
          curNode->size);
#endif
      curNode = freeBlock;  // curNode point to new chunk meta data
    }

    curNode = curNode->nextBlock;
  }

  return freeBlock;
}

void * ff_getBlock(size_t size) {
  memory_block_meta * freePtr = block_manager->freeListHead;
  while (freePtr != NULL) {
    if (freePtr->size >= size) {
      // size and startAddr can use previous info as it would not change
      // TODO: maintain the linked list
      freePtr->type = MEM_ALLOCATED;
      memory_block_meta * retChunk = removeFromList(freePtr);
#ifdef NDEBUG
      fprintf(stderr,
              "ff_getBlock(): get chunk from free list %p with size %zu\n",
              retChunk,
              retChunk->size);
#endif
      return retChunk->data;
    }

    freePtr = freePtr->nextBlock;
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
          "getBlock_info(): chunk %p, size: %zu, chunk type: %d, nextBlock: %p,  "
          "dataStartAddr: "
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

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
  if (block_manager->freeListHead == NULL) {
    ((memory_block_meta *)toAdd)->nextBlock = NULL;
    ((memory_block_meta *)toAdd)->prevBlock = NULL;
    block_manager->freeListHead = toAdd;

#ifdef NDEBUG
    fprintf(stderr,
            "insertToList(): adding new chunk %p with size %zu to free list\n",
            toAdd,
            ((memory_block_meta *)toAdd)->size);
    printList();
#endif

    return toAdd;
  }

  ((memory_block_meta *)toAdd)->nextBlock = block_manager->freeListHead;
  ((memory_block_meta *)toAdd)->prevBlock = NULL;
  ((memory_block_meta *)block_manager->freeListHead)->prevBlock = toAdd;
  block_manager->freeListHead = toAdd;

#ifdef NDEBUG
  fprintf(stderr,
          "insertToList(): adding new chunk %p with size %zu to free list\n",
          toAdd,
          ((memory_block_meta *)toAdd)->size);
  printList();
#endif

  return toAdd;
}

void * removeFromList(memory_block_meta * toRemove) {
  if (toRemove == block_manager->freeListHead) {
    block_manager->freeListHead = toRemove->nextBlock;
  }
  if (toRemove->prevBlock != NULL) {
    toRemove->prevBlock->nextBlock = toRemove->nextBlock;
  }
  if (toRemove->nextBlock != NULL) {
    toRemove->nextBlock->prevBlock = toRemove->prevBlock;
  }

  toRemove->nextBlock = NULL;
  toRemove->prevBlock = NULL;

#ifdef NDEBUG
  fprintf(stderr,
          "removeFromList(): remove chunk %p with size %zu\n",
          toRemove,
          ((memory_block_meta *)toRemove)->size);
  printList();
#endif
  return toRemove;
}

void printList() {
  memory_block_meta * ptr = block_manager->freeListHead;
  while (ptr != NULL && ptr->nextBlock != NULL) {
    fprintf(stderr, "%p with size %zu ---->  ", ptr, ptr->size);
    ptr = ptr->nextBlock;
  }

  memory_block_meta * prePtr = ptr;
  if (ptr != NULL) {
    fprintf(stderr, "%p with size %zu ---->  ", ptr, ptr->size);
  }

  fprintf(stderr, "NULL\n");

  while (prePtr != NULL) {
    fprintf(stderr, "%p with size %zu ---->  ", prePtr, prePtr->size);
    prePtr = prePtr->prevBlock;
  }
  fprintf(stderr, "NULL\n");
}

void * mergeBlock(memory_block_meta * freeBlock) {
  merge_times += 1;  // TODO remove this
  memory_block_meta * curNode = block_manager->freeListHead;

  while (curNode != NULL) {
    void * freeBlockStart = freeBlock;
    void * freeBlockEnd = freeBlockStart + sizeof(memory_block_meta) + freeBlock->size;
    void * startCurNode = curNode;
    void * endCurNode = startCurNode + sizeof(memory_block_meta) + curNode->size;

    if ((freeBlockStart == endCurNode) ||
        (startCurNode == freeBlockEnd)) {  // memory adjacent

      void * prevBlock = curNode->prevBlock;
      void * nextBlock = curNode->nextBlock;
      curNode = removeFromList(curNode);
      if (freeBlockStart == endCurNode) {
        void * tmp = curNode;
        curNode = freeBlock;
        freeBlock = tmp;
      }

      freeBlock->nextBlock = nextBlock;
      freeBlock->prevBlock = prevBlock;
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
      freePtr->type = MEM_ALLOCATED;
      memory_block_meta * retChunk = removeFromList(freePtr);
      void * remainChunk = sliceChunk(retChunk, size);
      if (remainChunk != NULL) {
        insertToList(remainChunk);
      }
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
  memory_block_meta * newChunk = getNewBlock(size + sizeof(memory_block_meta));

  return newChunk->data;
}

void * getNewBlock(size_t size) {
  memory_block_meta * newChunk = sbrk(size);
  newChunk->size = size - sizeof(*newChunk);
  newChunk->type = MEM_ALLOCATED;
  newChunk->nextBlock = NULL;
  newChunk->prevBlock = NULL;
  newChunk->data = (void *)newChunk + sizeof(memory_block_meta);
  block_manager->heap_size += size;

#ifdef NDEBUG
  getBlock_info(newChunk);
#endif

  return newChunk;
}

void * sliceChunk(memory_block_meta * chunk, size_t request) {
  assert(chunk != NULL);
  size_t remaining_size = (chunk->size >= request) ? chunk->size - request : 0;
  if (remaining_size <= sizeof(*chunk)) {
    return NULL;
  }

  chunk->size = request;
  chunk->type = MEM_ALLOCATED;
  // chunk->nextBlock = NULL;
  memory_block_meta * remain_chunk = (void *)chunk + sizeof(*chunk) + request;
  remain_chunk->size = remaining_size - sizeof(*remain_chunk);
  remain_chunk->type = MEM_FREE;
  remain_chunk->nextBlock = NULL;
  remain_chunk->data = (void *)remain_chunk + sizeof(*remain_chunk);

#ifdef NDEBUG
  fprintf(stderr, "slicingChunk():\n");
  getBlock_info(chunk);
  getBlock_info(remain_chunk);
#endif

  return remain_chunk;
}

void getBlock_info(memory_block_meta * chunk) {
  assert(chunk != NULL);
  fprintf(stderr,
          "getBlock_info(): chunk %p, size: %zu, chunk type: %d, nextBlock: %p,  "
          "prevBlock: %p,"
          "dataStartAddr: "
          "%p\n",
          chunk,
          chunk->size,
          chunk->type,
          chunk->nextBlock,
          chunk->prevBlock,
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

unsigned long get_data_segment_size() {
  return block_manager->heap_size;
}

unsigned long get_data_segment_free_space_size() {
  memory_block_meta * ptr = block_manager->freeListHead;
  unsigned long free_space_size = 0;
  while (ptr != NULL) {
    free_space_size += ptr->size + sizeof(*ptr);
    ptr = ptr->nextBlock;
  }
  return free_space_size;
}

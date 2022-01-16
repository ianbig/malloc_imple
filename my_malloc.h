#ifndef _MY_MALLOC_H
#define _MY_MALLOC_H

#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
// meta information
enum MEM_TYPE { MEM_ALLOCATED, MEM_FREE };

typedef enum MEM_TYPE MEM_TYPE;

typedef struct memory_block_meta memory_block_meta;
struct memory_block_meta {
  size_t size;    // unsiged long type do not include meta data size
  MEM_TYPE type;  // specfiy which type
  memory_block_meta * nextBlock;
  memory_block_meta * prevBlock;
  void * data;  // start address of this memory block
};

struct memory_control_block {
  void * freeListHead;
  // TODO: maybe add some info to increase performace
};

typedef struct memory_control_block memory_control_block;

memory_control_block * block_manager = NULL;
// function for both first fit and best fit

/* ===== first fit ======*/
void * ff_malloc(size_t size);
void ff_free(void * toFree);
/*
  @ size: size_t
  return address of memory block
  use first fit algorithm to find the first fit block, if no such block exist allocate a new data chunk. the function return memory address of the reqeusted data chunk, including the meta data
 */
void * ff_getBlock(size_t size);
void init_memory_control_block();

// ff_debug info
void getBlock_info(memory_block_meta * chunk);

/* ===== best fit ======*/
void * bf_malloc(size_t size);
void bf_free(void * ptr);

/* ===== helper function  ======*/
// merge the adjacent block with freeBlock, and remove merged block in free list (TODO: try to refactor this since it violate SRP)
void * mergeBlock(memory_block_meta * freeBlock);
// return removed block
void * removeFromList(void * toRemove);
void * insertToList(void * toAdd);

/* debug function for linked list*/
void printList();
#endif

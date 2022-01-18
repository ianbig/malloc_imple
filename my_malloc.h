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
  size_t heap_size;
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
void * removeFromList(memory_block_meta * toRemove);
void * insertToList(void * toAdd);
/* @ chunk: data chunk to be sliced
   return remaining unused chunuk with its meta data is set, if the space is not enough for meta data the function return NULL

   slicChunk would split the passed in chunk to requested chunk and remaining chunk with meta data header, if the remaining chunk is not enough for wrtting meta head the NULL would be return
*/
void * sliceChunk(memory_block_meta * chunk, size_t request);
void * getNewBlock(size_t size);
unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();

/* debug function for linked list*/
size_t split_times = 0;
size_t merge_times = 0;
size_t insert_to_list_times = 0;
size_t remove_from_list_times = 0;
void printList();
#endif

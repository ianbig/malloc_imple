#ifndef _MY_MALLOC_H
#define _MY_MALLOC_H

#include <stddef.h>
#include <sys/types.h>
// meta information
enum MEM_TYPE { MEM_ALLOCATED, MEM_FREE };

typedef enum MEM_TYPE MEM_TYPE;

struct memory_block_meta {
  size_t size;    // unsigedn long
  MEM_TYPE type;  // specfiy which type
  void * nextBlcok;
  void * startAddr;  // start address of this memory block
};
typedef struct memory_block_meta memory_block_meta;

struct memory_control_block {
  void * freeListHead;
  // TODO: maybe add some info to increase performace
};

typedef struct memory_control_block memory_control_block;

memory_control_block * block_manager = NULL;
// first fit
void * ff_malloc(size_t size);
void ff_free(void * ptr);
void * findBlock();
void init_memory_control_block();

// best fit
void * bf_malloc(size_t size);
void bf_free(void * ptr);
#endif

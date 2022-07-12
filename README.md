# Memory Allocator: Sbrk Implementation
## Locked Based Implementation Detail
To design the memory allocation system call, it is important to manage the heap size for dynamic allocation. To do that, I can utilize the system call sbrk(), which would adjust the position of program break, a pointer to specify the end of data size. Once I handle the way to allocate heap space, I need to concern how to allocate the required amount of memory, i.e. dividing memory into chunks of data. To record the information for each memory chunk, I need a meta data to specify the following information:

```c
struct memory_block_meta {
  size_t size;    // unsiged long type do not include meta data size
  MEM_TYPE type;  // specfiy which type
  memory_block_meta * nextBlock; // next block in free list
  memory_block_meta * prevBlock; // previous block in free list
  void * data;  // start address of this memory block
};
```

### Freeing Memory
When freeing the memory, I do not actually decrease the heap size (since this would be to inefficiency by calling sbrk multiple times), I instead put the freed memory to the abstract linked list. Also, if the newly added free section has an adjacent chunk in the free list. I merge both chunks.

Besides merging blocks when free, I also slice the free chunk when allocated. If the return chunk from the free list is larger than the requested chunk, the chunk needed to be sliced. However, the way to slice the memory would result in significant performance difference even though with the $O(n)$ algorithmatically result

#### Performance Benchmark
I implemented two ways of slicing a chunk and adding a free chunk to the linked list. The first one is always added freed chunk to the head of list and search through chunks in the free list. However, the performance is very bad even with O(n) complexity.


| Pattern | small range | equal range | large range
| ----- | ----- | ----- | ---
First Fit | 21 sec | 1400 sec |140 sec
Best fit | NA | NA |NA

Figure 1. performance of first approach: best Fit is NA since this approach do not implement

The reason for this result is that when searching through the linked list when merged, the linked list is in a different location. Hence, the locality of the data would be poor, resulting in cache miss (which is a huge overhead for system, sometimes the data might even in disk which require more than a million cycle to fetch data)

With the locality in mind, I redesign my algorithm. When inserting a free list, I insert in an ordering way, which enables me to merge adjacent data chunks with O(1) complexity.


| Pattern | small range | equal range | large range
| --- | ---- | ---- | ---
First Fit | 10.1 sec | 23.99 | 62.1 sec
Best fit | 3.3 sec | 24.2 sec | 65 sec

Figure 2. time for method 2

Pattern | small range | equal range | large range
| ---- | ------ | ---- | ----
First Fit | 0.06 | 0.45 | 0.09 
Best fit | 0.02 | 0.45 | 0.04

Figure 3. Fragmentation for method 2

The performance in time is fairly the same in equal range and large range for both allocation methods. For equal range, the free memory chunk is the same, thus regardless of best fit or first fit they both need to search through the free list to find appropriate size. For large ranges, the space range is fairly large; thus, it is likely that both methods would find the appropriate size easily. But for small ranges the best bit is to outperform the first fit. The reason for this is that the best fit is always finding the appropriate size.  Hence, it would reduce the time to slice and insert to a linked list.


## Thread Local Storage Implementation
For the unlock version, I implement the thread_local storage technique, where I have each thread to handle its own free space. In this way, the threads do not need to communicate with each other.

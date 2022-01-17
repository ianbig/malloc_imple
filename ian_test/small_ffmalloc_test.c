#include <stdio.h>

#include "my_malloc.h"
int main() {
  int * ptr = ff_malloc(sizeof(*ptr) * 10);

  for (int i = 0; i < 10; i++) {
    ptr[i] = i;
  }

  ff_free(ptr);

  // use free list
  ptr = ff_malloc(sizeof(*ptr) * 10);
  for (int i = 0; i < 10; i++) {
    ptr[i] = i;
  }

  printf("==== print list =====\n");
  for (int i = 0; i < 10; i++) {
    printf("%d, ", ptr[i]);
  }
  printf("\n");
}

#include <stdio.h>

#include "my_malloc.h"
int main() {
  char * ptr[3];

  fprintf(stderr,
          "\n====== Tesing Mergeing 3 element free non adjacnet chunk, then free chun "
          "that is adjacent to both ========\n");
  for (int i = 0; i < 3; i++) {
    ptr[i] = ff_malloc(sizeof(*ptr[i]) * 128);
  }

  ff_free(ptr[2]);
  ff_free(ptr[0]);
  ff_free(ptr[1]);

  fprintf(stderr, "\n======Adding two more element smaller than memory chunk========\n");
  for (int i = 0; i < 2; i++) {
    ptr[i] = ff_malloc(sizeof(*ptr[i]) * 128);
  }

  ff_free(ptr[0]);
  ff_free(ptr[1]);
}

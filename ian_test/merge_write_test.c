#include "my_malloc.h"

int main() {
  int * ptr[3];

  for (int i = 0; i < 3; i++) {
    ptr[i] = ff_malloc(sizeof(int *) * 20);
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 20; j++) {
      ptr[i][j] = i * j;
    }
  }

  for (int i = 0; i < 3; i++) {
    ff_free(ptr[i]);
  }

  char * cptr[3];
  for (int i = 0; i < 3; i++) {
    cptr[i] = ff_malloc(sizeof(char *) * 10);
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) {
      cptr[i][j] = 'a';
    }
  }

  for (int i = 0; i < 3; i++) {
    ff_free(cptr[i]);
  }

  return 0;
}

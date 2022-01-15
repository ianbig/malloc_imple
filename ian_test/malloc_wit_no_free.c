#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_malloc.h"

#define KB 1 << 10
#define MB 1 << 20
#define GB 1 << 30

int main() {
  int * ptrInt = ff_malloc(10 * sizeof(int));
  for (size_t i = 0; i < 10; i++) {
    ptrInt[i] = i;
  }

  for (size_t i = 0; i < 10; i++) {
    assert(ptrInt[i] == i);
    // printf("ptrInt %zu get val %d\n", i, ptrInt[i]);
  }

  char * ptrChar = ff_malloc(1 * sizeof(char));
  ptrChar[0] = 'a';
  assert(ptrChar[0] == 'a');

  double * dArray = ff_malloc(30 * sizeof(double));
  for (size_t i = 0; i < 30; i++) {
    dArray[i] = i;
  }

  for (size_t i = 0; i < 30; i++) {
    assert(dArray[i] == i);
    // printf("dArray %zu get val %f\n", i, dArray[i]);
  }

  // large memory allocation
  size_t sz = KB;
  int * kb = ff_malloc(sz * sizeof(int));
  for (size_t i = 0; i < sz; i++) {
    kb[i] = i;
  }

  for (size_t i = 0; i < sz; i++) {
    assert(kb[i] == i);
  }

  sz = MB;
  int * mb = ff_malloc(sz * sizeof(int));
  for (size_t i = 0; i < sz; i++) {
    mb[i] = i;
  }

  for (size_t i = 0; i < sz; i++) {
    assert(mb[i] == i);
  }

  /*
  sz = GB;
  int * gb = ff_malloc(sz * sizeof(int));
  for (size_t i = 0; i < sz; i++) {
    gb[i] = i;
  }

  for (size_t i = 0; i < sz; i++) {
    assert(gb[i] == i);
  }
  */

  return 0;
}

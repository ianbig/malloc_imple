#include <assert.h>
#include <stdio.h>

int main() {
#ifndef NDEBUG
  fprintf(stderr, "My trace message\n");
#endif
  printf("Normal flow\n");
}

#include <stdio.h>
#include <unistd.h>

int main() {
  /* for (int i = 0; i < 2; i++) { */
  /*   void * addr = sbrk(10); */
  /*   printf("address: %p with size 10 is allocated\n", addr); */
  /* } */

  void * addr = sbrk(10);
  printf("address: %p\n", addr);

  addr = sbrk(0);
  printf("address: %p\n", addr);

  /* addr = sbrk(1); */
  /* addr = sbrk(0); */
  /* printf("address: %p with size 1 is allocated\n", addr); */

  return 0;
}

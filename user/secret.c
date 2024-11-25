#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"


int
main(int argc, char *argv[])
{
  if(argc != 2){
    printf("Usage: secret the-secret\n");
    exit(1);
  }
  char *end = sbrk(PGSIZE*32);
  end = end + 9 * PGSIZE;

  // strcpy(end, "my very very very secret pw is:   ");
  // strcpy(end+32, argv[1]);

  char find_str[41];
  memmove(find_str, "my very very very secret pw is:   ", 32);
  memmove(find_str + 32, argv[1], 8);
  memmove(end, find_str, 40);

  // char find_str[41];
  // strcpy(find_str, "my very very very secret pw is: ");
  // strcpy(find_str + 32, argv[1]);
  // // printf("%s\n", find_str);
  // memmove(end, find_str, 40);

  exit(0);
}


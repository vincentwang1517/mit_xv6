#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define PREFIX "my very very very secret pw is: "
#define PREFIX_LEN 32

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)

  char *end = sbrk(32 * PGSIZE);

  // Visit every created page and check if there is key string "my very" in the beginning
  // Inside page the memory is continuous. We assume the page in which secret.c stored password will be reused.
  for (int addr_offset = 0; addr_offset < 64 * PGSIZE - 32; addr_offset++)
  {
    if (memcmp(PREFIX, end + addr_offset, PREFIX_LEN) == 0) {

      char test[41];
      test[40] = 0;
      memcpy(test, end + addr_offset, 40);
      printf("%s\n", test);

      write(2, end + addr_offset + 32, 8);
      exit(0);
    }
  }

  exit(0);
}

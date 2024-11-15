#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void findPrime(int) __attribute__((noreturn));

int CAP = 280;

void findPrime(int left_p)
{
  // the first read will be the prime
  int prime;
  read(left_p, (void*)&prime, sizeof(prime));
  fprintf(1, "prime %d\n", prime);

  // if I can read the next member (then it must be a prime)
  // then we need to create a next level process
  int entry;
  if (read(left_p, (void*)&entry, sizeof(entry)) != 0) 
  {
    // create a pipe for right neighbor and write the first number
    int p[2];
    pipe(p);
    write(p[1], (void*)&entry, sizeof(entry));

    // fork a child
    int pid;
    pid = fork();

    if (pid == 0) {
      close(p[1]);

      // per process at maximum can have 16 file descriptors open
      close(left_p);

      findPrime(p[0]);
      close(p[0]);
    }
    else {
      close(1);

      close(p[0]);
      // keep read from left nieghbor and write to right neighbor
      while (read(left_p, (void*)&entry, sizeof(entry)) != 0) {
        if (entry % prime != 0)
          write(p[1], (void*)&entry, sizeof(entry));
      }

      close(p[1]);
    }
  }

  close(left_p);

  exit(0);
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);

  close(0);
  close(2);

  int pid;
  pid = fork();

  if (pid == 0) {
    close(p[1]);
    findPrime(p[0]);
    close(p[0]);
  }
  else { // parent
    close(1);

    close(p[0]);
    // send 2 - 280
    for (int i = 2; i <= CAP; i++) {
      write(p[1], (void*)&i, sizeof(i));
    }
    close(p[1]);
  }


  exit(0);
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void findPrime(int (*left_p)[2]) __attribute__((noreturn));

int CAP = 37;

void findPrime(int (*left_p)[2])
{
  // the first read will be the prime
  int prime;
  read(*left_p[0], (void*)&prime, sizeof(prime));
  fprintf(1, "prime: %d\n", prime);

  // if I can read the next member (then it must be a prime)
  // then we need to create a next level process
  int entry;
  if (read(*left_p[0], (void*)&entry, sizeof(entry)) != 0) 
  {
    // create a pipe for right neighbor and write the first number
    int p[2];
    pipe(p);
    write(p[1], (void*)&entry, sizeof(entry));

    int pid;
    pid = fork();

    if (pid == 0) {
      close(p[1]);
      findPrime(&p);
    }
    else {
      close(p[0]);
      // keep read from left nieghbor and write to right neighbor
      while (read(*left_p[0], (void*)&entry, sizeof(entry)) != 0) {
        if (entry % prime != 0)
          write(p[1], (void*)&entry, sizeof(entry));
      }
      close(p[1]);
    }
  }

  close(*left_p[0]);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);

  int pid;
  pid = fork();

  if (pid == 0) {
    close(p[1]);
    findPrime(&p);
  }
  else { // parent
    close(p[0]);
    // send 2 - 280
    for (int i = 2; i <= CAP; i++) {
      write(p[1], (void*)&i, sizeof(i));
    }
    close(p[1]);
  }

  exit(0);
}
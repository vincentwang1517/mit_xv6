#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // pipe
  int p[2];
  pipe(p);

  // fork
  int pid;
  pid = fork();

  if (pid == 0) { // child
    // read buffer
    char buf[1];
    read(p[0], buf, 1);

    // print and close pipe read
    fprintf(1, "%d: received ping\n", getpid());
    close(p[0]);

    // pipe-write and close
    write(p[1], buf, 1);
    close(p[1]);

    exit(0);
  }
  else { // parent
    // pipe-write and close
    char buf[1];
    write(p[1], buf, 1);
    close(p[1]);

    // wwait for child process 
    wait(0);

    // read pipe, print, and close
    read(p[0], buf, 1);
    fprintf(1, "%d: received pong\n", getpid());

    close(p[0]);

  }

  exit(0);
}

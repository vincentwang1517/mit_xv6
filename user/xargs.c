#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXCMDLEN 16

int
main(int argc, char *argv[])
{
  // error handling
  if (argc < 2) {
    fprintf(2, "xargs <cmd> ...\n");
    exit(0);
  }
  if (argc >= MAXCMDLEN) {
    fprintf(2, "xargs max cmd len %d\n", MAXCMDLEN);
    exit(0);
  }

  // find the command
  char *cmd[MAXCMDLEN], **p_cmd;
  p_cmd = cmd;
  for (int i = 1; i < argc; i++) {
    *p_cmd = argv[i];
    p_cmd++;
  }
  *(p_cmd+1) = 0; // terminate commands

  // read stdin: buf with len(p-buf-1)
  char buf[512], *p;
  p = buf;
  while (read(0, p, 1) == 1) 
  {
    if (*p == '\n') 
    {
      *p = 0;
      *p_cmd = buf;

      int pid;
      pid = fork();
      if (pid == 0) {
        exec(cmd[0], cmd);
      }
      else {
        wait(0);
      }

      p = buf;
    }
    else {
      p++;
    }
  }

  exit(0);
}
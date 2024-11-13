#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char* path, char* key)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // fstat the path
  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type)
  {
  case T_DEVICE: break;

  case T_FILE:
    // split filename and save to buf
    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;
    memmove(buf, p, strlen(p));
    // if match the key, print the path
    if (strcmp(buf, key) == 0) {
      fprintf(1, path, strlen(path));
    }
    break;

  case T_DIR:
    // read all following files
    while(read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0) 
        continue;
      fprintf(1, "%s\n", de.name);
    }
    break;
  }

  return;
}


int
main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(2, "find <dir> <keyword>\n");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
}
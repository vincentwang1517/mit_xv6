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
    strcpy(buf, p); // will copy 0
    // memmove(buf, p, strlen(p));

    // if match the key, print the path
    if (strcmp(buf, key) == 0) {
      fprintf(1, "%s\n", path);
    }
    break;

  case T_DIR:
    // put path into buf
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(path);
    *p++ = '/'; // replace 0 with '/'

    // read all following files
    while(read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0) 
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;

      // compose the path
      strcpy(p, de.name);
      find(buf, key);
    }
    break;
  }
  
  // has to close bc one process can have at max 16 fd
  close(fd);

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
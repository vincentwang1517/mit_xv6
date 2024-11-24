## GDB

```bash
make CPUS=1 qemu-gdb
riscv64-elf-gdb

# cmd
layout split
```

```bash
p /x p->trapframe
$1 = 0x87f56000 # vitual address kernel data
p /x p->trapframe->a7
$2 = 0x7 # exec
p /x &p->trapframe->a7
$3 = 0x875f60a8 # virtual address to p->trapframe->a7
```

## How Sys Call Begin Called

### User Space

Take `exec` for example

```c
exec(nargv[0], nargcv);
```

`user/user.h`

```c
int exec(const char*, char**);
```

*How does user/user.h interact with others*?

In `user/usys.S`, load the syscall number and enter kernek mode

```c
#include "kernel/syscall.h"

.global exec
exec:
 li a7, SYS_exec // load integer
 ecall
 ret
```

### Kernel Space

In `kernel/syscall.c`, it provides a syscall look-up table and call the syscall.

```c
static uint64 (*syscalls[])(void) = 
{
...
[SYS_exec]    sys_exec,
...
};

// defined in defs.h
void
syscall(void)
{
  int num;
  struct proc *p = myproc();

  num = p->trapframe->a7;
  p->trapframe->a0 = syscalls[num]();
}
```

In `kernel/sysfile.c` or `kernel/sysproc.c`, a system call is implemented.

```c
uint64
sys_exec(void)
{
  // 1. retrieve system call arguments from user space
  // 2. implementation
}
```

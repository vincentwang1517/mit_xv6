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
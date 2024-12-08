# LAB - PGTBL

- A page table is 4096 Bytes = 512 * 8 Bytes... So it has 512 PTEs in one page
- `kalloc()` return a **pa** of a free page 

## Speed up system calls

- trapframe & usyscall both use one page:

```c
// kernel/memlayout.h
#define TRAPFRAME (TRAMPOLINE - PGSIZE)
#define USYSCALL (TRAPFRAME - PGSIZE)
```

>*TIP: Think of it as a special trapframe*

**When and Where** a process being allocated? - init or fork

```c
// kernel/proc.c
static struct proc*
allocproc(void)
{
  // + find unused proc in proc[NPROC]
  // + Use kalloc() to allocate a trapframe page
  // + Use proc_pagetable(p) to allocate a page table
}
```

**What does `proc_pagetable(p)` do?

```c
// kernel/proc.c
// Create a user page table for a given process, with no user memory,
// but with trampoline and trapframe pages.
pagetable_t
proc_pagetable(struct proc *p)
{
  // + Create a random page table as the root dir
  pagetable = uvmcreate();
  // + Map TRAMPOLINE (va) to trampoline (pa) --- mappages
  // + Map TRAPFRAME (va) to trapframe (pa) --- mappages
}
```

## Use superpages 

> Any level of PTE may be a leaf PTE, so in addition to 4 KiB pages, Sv39 supports 2 MiB megapages and 1 GiB gigapages, each of which must be virtually and physically aligned to a boundary equal to its size.

- `sbrk`: `sys_sbrk` &#8594; `growproc(n)` &#8594; `uvmalloc()` & `uvmdealloc()` 

### Solution

```text
----- PHYSTOP
save for megapage
----- SUPERSTOP
save for page
----- KERNEL BASE
```

### kalloc.c

A linked list to store free page:

```c
struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;
```

Initialize the free list:

```c
void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p); // Here call memset(pa, 1, PGSIZE) and add the p the kmem.freelist
}
```

### Where to allocate the superpage?

In `uvmalloc()`
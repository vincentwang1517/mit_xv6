# Lab: System Calls

## Attack xv6

In `secret.c`, `sbrk()` increases continuous heap memory (from the end of parent's memory)


> Copy-On-Write (COW):
>  
> Modern operating systems implement copy-on-write to optimize performance. This means:
> - Initially, the parent and child share the same physical memory pages (including the heap).
> - When either process attempts to modify the memory, a private copy of the modified pages is created for that process.
> - This ensures that changes in the heap of one process (e.g., expanding it with sbrk) do not affect the other.


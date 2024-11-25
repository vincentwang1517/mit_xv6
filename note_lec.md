## Lecture 4 Page Tables

- HW support: MMU
- Every app has its own map: `satp` stored the map address and mmu does translation
- There are 2^64 addresses (xv6 register is 64-bit) - need **page**

**Page: 4096 bytes**

Decode the address:

- \[39, 64\]: EXT
- \[12, 38\]: index
- \[0 , 11\]: offset
# build

Run make command without any options.

```
$ make
```

# run

Run command with default parameter.

```
$ ./pg
```

Run command with parameters.

```
$ ./pg -p 10 -m 8 -l 100
```

# options

* -p [PROCESS COUNT]
* -l [LOOP COUNT]
* -m [MEGA BYTES]

# example

Run command with 3thread, 1 loop, and 8MiB memory.

```
masami@saga:~/codes/how_paging_works (master %)$ ./pg -p 3 -l 1 -m 8
{
"physical memory":{"size":"8MiB","range": {"start":"0x7f9bd251b000","end":"0x7f9bd2d1b000"},"page flames":2048},
"page data": [
{"pid":1,"pgd_idx":32,"pud_idx":0,"pmd_idx":0,"pte_idx":0,"vaddr":"0x100000000000","paddr":"0x7f9bd251b000"},
{"pid":2,"pgd_idx":32,"pud_idx":0,"pmd_idx":0,"pte_idx":0,"vaddr":"0x100000000000","paddr":"0x7f9bd251c000"},
{"pid":3,"pgd_idx":32,"pud_idx":0,"pmd_idx":0,"pte_idx":0,"vaddr":"0x100000000000","paddr":"0x7f9bd251d000"}
]
}
masami@saga:~/codes/how_paging_works (master %)$
```

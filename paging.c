#include "./paging.h"
#include "./memory.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "./page_allocator.h"

static struct page_frame *page_frames;

void paging_init(void)
{
	void *phy_mem = get_physical_memory();

	page_frames = malloc(sizeof(struct page_frame) * num_page_frames);
	assert(page_frames);

	for (unsigned long i = 0; i < num_page_frames; i++) {
		unsigned long addr = (unsigned long) phy_mem + (i * PAGE_SIZE);
		page_frames[i].used = false;
		page_frames[i].addr = addr;
	}
}

struct page_frame *get_page_frames(void)
{
	return page_frames;
}

void __do_page_fault(int pid, pgd_t *pgd, unsigned long addr)
{
	int pgd_idx = pgd_index(addr);
	pud_t **pud = pud_offset(pgd, pgd_idx);
	if (pud_none(*pud))
		*pud = pud_alloc();

	int pud_idx = pud_index(addr);
	pmd_t **pmd = pmd_offset(*pud, pud_idx);
	if (pmd_none(*pmd))
		*pmd = pmd_alloc();

	int pmd_idx = pmd_index(addr);
	pte_t **pte = pte_offset(*pmd, pmd_idx);

	if (pte_none(*pte))
		*pte = pte_alloc();

	char *chunk = (char *) get_free_page();
	if (!chunk)
		pthread_exit(NULL);

	memset(chunk, 0x0, PAGE_SIZE);

	int pte_idx = pte_index(addr);
	*(*pte + pte_idx) = (unsigned long) chunk;

	sprintf(chunk, "{\"pid\":%d,\"pgd_idx\":%d,\"pud_idx\":%d,\"pmd_idx\":%d,\"pte_idx\":%d,\"vaddr\":\"0x%lx\",\"paddr\":\"%p\"}", pid, pgd_idx, pud_idx, pmd_idx, pte_idx, addr, chunk);
}

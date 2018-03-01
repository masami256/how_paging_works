#ifndef PG_PAGING_H
#define PG_PAGING_H

#include "./pg.h"
#include "./util.h"
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef unsigned long pgd_t;
typedef unsigned long pud_t;
typedef unsigned long pmd_t;
typedef unsigned long pte_t;

struct page_frame {
	bool used;
	unsigned long addr;
};

#define PAGE_SHIFT	12
#define PMD_SHIFT	21
#define PUD_SHIFT	30
#define PGDIR_SHIFT	39

#define PTRS_PER_PGD	512
#define PTRS_PER_PUD	512
#define PTRS_PER_PMD	512
#define PTRS_PER_PTE	512

#define page_table_none(data) (data == NULL)
#define pud_none(pud) page_table_none(pud)
#define pmd_none(pmd) page_table_none(pmd)
#define pte_none(pte) page_table_none(pte)

#define pgd_index(address) (((address) >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1))

static inline unsigned long pud_index(unsigned long address)
{
	return (address >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
}

static inline unsigned long pmd_index(unsigned long address)
{
	return (address >> PMD_SHIFT) & (PTRS_PER_PMD - 1);
}

static inline unsigned long pte_index(unsigned long address)
{
	return (address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
}

static inline pud_t **pud_offset(pgd_t *pgd, unsigned long idx)
{
	return (pud_t **) &pgd[idx];
}

static inline pmd_t **pmd_offset(pud_t *pud, unsigned long idx)
{
	return (pmd_t **) &pud[idx];
}

static inline pte_t **pte_offset(pmd_t *pmd, unsigned long idx)
{
	return (pte_t **) &pmd[idx];
}

static inline void *page_table_alloc(void)
{
	void *p = pg_aligned_alloc(PAGE_SIZE, PAGE_SIZE);
	assert(p);

	memset(p, 0x0, PAGE_SIZE);

	return p;
}

static inline pgd_t *pgd_alloc(void)
{
	return page_table_alloc();
}

static inline pud_t *pud_alloc(void)
{
	return page_table_alloc();
}

static inline pmd_t *pmd_alloc(void)
{
	return page_table_alloc();
}

static inline pte_t *pte_alloc(void)
{
	return page_table_alloc();
}


void paging_init(void);
struct page_frame *get_page_frames(void);
void __do_page_fault(int pid, pgd_t *pgd, unsigned long addr);

#endif // PG_PAGING_H

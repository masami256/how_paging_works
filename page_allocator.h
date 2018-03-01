#ifndef PG_PAGE_ALLOCTOR_H
#define PG_PAGE_ALLOCATOR_H

#include "./memory.h"

typedef unsigned long gfp_t;

#define get_free_page() __get_free_pages(0, 0)
extern unsigned long __get_free_pages(gfp_t gfp_mask, unsigned int order);

#endif // PG_PAGE_ALLOCATOR_H

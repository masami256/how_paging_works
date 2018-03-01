#include "./memory.h"
#include "./util.h"
#include "./paging.h"
#include <assert.h>

static void *physical_memory;

void setup_physical_memory(void)
{
	physical_memory = pg_aligned_alloc(PAGE_SIZE, PHYSICAL_MEMORY_SIZE);
	assert(physical_memory);
}

void *get_physical_memory(void)
{
	return physical_memory;
}

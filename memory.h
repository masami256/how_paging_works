#ifndef PG_MEMORY_H
#define PG_MEMORY_H

#include "./pg.h"

#define byte_to_KiB(x) (x / (1 << 10))
#define byte_to_MiB(x) (x / (1 << 20))

void setup_physical_memory(void);
void *get_physical_memory(void);

// physical memory size is 1MiB
extern int megabytes;
#define PHYSICAL_MEMORY_SIZE (1 << 20) * megabytes

#define num_page_frames (PHYSICAL_MEMORY_SIZE / PAGE_SIZE)

#endif // PG_MEMORY_H

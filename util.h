#ifndef PG_UTIL_H
#define PG_UTIL_H

#include <stdlib.h>

static inline void *pg_aligned_alloc(size_t align, size_t size)
{
	void *p = NULL;
	posix_memalign(&p, align, size);
	return p;
}


#endif // PG_UTIL_H

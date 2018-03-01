#include "./page_allocator.h"
#include "./memory.h"
#include "./paging.h"
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

static pthread_mutex_t pfn_lock = PTHREAD_MUTEX_INITIALIZER;

static unsigned long find_free_page(void)
{
	int ret;

	ret = pthread_mutex_lock(&pfn_lock);
	assert(!ret);

	struct page_frame *pfns = get_page_frames();

	for (unsigned long i = 0; i < num_page_frames; i++) {
		struct page_frame *pfn = &pfns[i];

		if (!pfn->used) {
			pfn->used = true;
			pthread_mutex_unlock(&pfn_lock);
//			printf("[*]found free page 0x%lx at pfn index %ld\n", pfn->addr, i);
			return pfn->addr;
		}
	}

	pthread_mutex_unlock(&pfn_lock);
	printf("no more free pages\n");
	return 0;
}

unsigned long __get_free_pages(gfp_t gfp_mask, unsigned int order)
{
	gfp_mask = 0;
	order = 0;

	return find_free_page();
}

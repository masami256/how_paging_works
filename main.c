#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "./paging.h"
#include "./memory.h"
#include "./util.h"

struct process_startup_args {
	int pid;
	int nloop;
};

// test address
#define BASE_USER_SPACE_ADDRESS 0x100000000000

static void *process(void *arg)
{
	struct process_startup_args *argp = arg;
	int pid = argp->pid;
	int nloop = argp->nloop;

	pgd_t *pgd = pgd_alloc();

	for (int i = 0; i < nloop; i++) {
		unsigned long addr = BASE_USER_SPACE_ADDRESS + (i * 4096);

		__do_page_fault(pid, pgd, addr);
	}

	return NULL;
}

static void usage(char *prog)
{
	fprintf(stderr, "usage: %s [-p process count] [-l loop count] [-m megabytes]\n", prog);
	fprintf(stderr, "default: 2 processes, 10 loop, 4MiB\n");
}

int megabytes = 4;

static void dump_memory(void)
{
	void *physical_memory = get_physical_memory();

	puts("{");
	printf("\"physical memory\":{");
	printf("\"size\":\"%dMiB\",", byte_to_MiB(PHYSICAL_MEMORY_SIZE));
	printf("\"range\": {");
	printf("\"start\":\"%p\",", physical_memory);
	printf("\"end\":\"%p\"},", physical_memory + PHYSICAL_MEMORY_SIZE);
	printf("\"page flames\":%d},\n", num_page_frames);
	printf("\"page data\": [\n");
	for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i+= PAGE_SIZE) {
		char *p = physical_memory + i;
		char *next = physical_memory + (i + PAGE_SIZE);

		if (*p) {
			printf("%s", p);

			if (*next)
				printf(",\n");
			else
				printf("\n");
		}
	}

	printf("]\n}\n");
}

int main(int argc, char **argv)
{
	int nproc = 2;
	int nloop = 10;
	int opt;

	pthread_t *threads;

	while ((opt = getopt(argc, argv, "hp:l:m:")) != -1) {
		switch (opt) {
		case 'p':
			nproc = atoi(optarg);
			break;
		case 'l':
			nloop = atoi(optarg);
			break;
		case 'm':
			megabytes = atoi(optarg);
			break;
		case 'h':
			usage(argv[1]);
			exit(0);
		default:
			printf("[*]unknown option %c\n", opt);
			usage(argv[0]);
			exit(-1);
		}
	}

	setup_physical_memory();
	void *physical_memory = get_physical_memory();

	paging_init();

	threads = malloc(sizeof(pthread_t) * nproc);
	assert(threads);

	memset(threads, 0x0, sizeof(pthread_t) * nproc);

	for (int i = 0; i < nproc; i++) {
		struct process_startup_args *arg = malloc(sizeof(struct process_startup_args));
		assert(arg);

		arg->pid = i + 1;
		arg->nloop = nloop;

		int ret = pthread_create(&threads[i], NULL, process, arg);
		assert(!ret);
	}


	for (int i = 0; i < nproc; i++)
		pthread_join(threads[i], NULL);


	dump_memory();

	free(threads);
	free(physical_memory);

	return 0;
}

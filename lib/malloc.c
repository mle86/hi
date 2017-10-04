#ifndef __MALLOC_C
#define __MALLOC_C
#include "malloc.h"

#ifdef PROGNAME
  #define _MALLOC_ERRMSG PROGNAME": Out of memory, could not allocate %zi bytes.\n"
#else
  #define _MALLOC_ERRMSG           "Out of memory, could not allocate %zi bytes.\n"
#endif

void* Malloc (size_t size) {
	register void* ptr;
	if (size < 1)
		return NULL;
	ptr = malloc(size);
	if (ptr == NULL) {
		fprintf(stderr, _MALLOC_ERRMSG, size);
		abort();
	}
	return ptr;
}

#endif /* __MALLOC_C */

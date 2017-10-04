#ifndef __MALLOC_C
#define __MALLOC_C
#include "malloc.h"


#define _MALLOC_ERRMSG "Out of memory, could not allocate %i bytes \n"

void* NullMalloc (size_t size) {
	register void* ptr;
	if (size < 1)
		return NULL;
	ptr = calloc(size, 1);
	if (ptr == NULL) {
		printf(_MALLOC_ERRMSG, size);
		exit(1);
	}
	return ptr;
}

void* Malloc (size_t size) {
	register void* ptr;
	if (size < 1)
		return NULL;
	ptr = malloc(size);
	if (ptr == NULL) {
		printf(_MALLOC_ERRMSG, size);
		exit(1);
	}
	return ptr;
}

void MultiFree (void* First, ...) {
	register void* ptr;
	va_list va;
	va_start(va, First);
	if (First)
		free(First);
	while(( ptr = va_arg(va, void*) ))
		free(ptr);
	va_end(va);
}


#endif // __MALLOC_C

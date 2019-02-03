/*
 * This file is part of the 'hi' project
 * (see https://github.com/mle86/hi).
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2007-2019 Maximilian Eul
 */

#include <stdlib.h>
#include <err.h>
#include "exit.h"
#include "malloc.h"

// Like malloc(3), but calls abort() if the allocation fails.
void* Malloc (size_t size) {
	if (size < 1)
		return NULL;
	void* ptr = malloc(size);
	if (ptr == NULL && size > 0) {
		errx(EXIT_OUT_OF_MEMORY, "out of memory");
	}
	return ptr;
}

// Like realloc(3), but calls abort() if the allocation fails.
void* Realloc (void* ptr, size_t newsize) {
	ptr = realloc(ptr, newsize);
	if (ptr == NULL && newsize > 0) {
		errx(EXIT_OUT_OF_MEMORY, "out of memory");
	}
	return ptr;
}


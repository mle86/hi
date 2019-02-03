/**
 * Memory allocation helper functions.
 *
 * This file is part of the 'hi' project
 * (see https://github.com/mle86/hi).
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2007-2019 Maximilian Eul
 */

#ifndef _MALLOC_H
#define _MALLOC_H

#include <stddef.h>


/** Like malloc(3), but calls abort() if the allocation fails.  */
void* Malloc (size_t size);

/** Like realloc(3), but calls abort() if the allocation fails.  */
void* Realloc (void* ptr, size_t size);


#endif  // _MALLOC_H

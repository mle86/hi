#ifndef __MALLOC_H
#define __MALLOC_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "compiler.h"


/* Extended memory functions.
 *
 * Malloc() simply returns NULL if size is lower than 1
 * and terminates program execution with an error message if the
 * memory couldn't be allocated.
 * NullMalloc() is identical to Malloc() except for its behaviour
 * to fill the allocated memory with NUL bytes.
 */

void* Malloc     (size_t size);
void* NullMalloc (size_t size);

void MultiFree (void* first, ...) __ATTR_sentinel;


#endif /* __MALLOC_H */

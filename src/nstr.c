#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "malloc.h"
#include "nstr.h"

#define append_reserve 2048

#define realloc_nstr(ptr, newlen)  do{  \
		const size_t newstructsize = sizeof(size_t) + (newlen) + 1;	\
		(ptr) = Realloc((ptr), newstructsize);			   	\
	}while(0)


nstr* nstr_init (size_t size) {
	const size_t slen = sizeof(size_t) + size + 1;
	nstr* n = Malloc(slen);
	n->length = 0;
	n->buffer[0] = '\0';  // nstr are ALWAYS terminated!
	return n;
}

inline void nstr_truncate (nstr* s) {
	if (!s)
		return;
	s->buffer[0] = '\0';
	s->length    = 0;
}

inline void nstr_destroy (nstr* s) {
	free(s);
}

void nstr_resize (nstr* *dest, size_t newsize) {
	if (!dest)
		return;
	if (newsize <= 0) {
		free(*dest);
		*dest = NULL;
	}

	realloc_nstr(*dest, newsize);

	if ((*dest)->length > newsize) {
		// Truncate
		(*dest)->length = newsize;
	} else if ((*dest)->length == newsize) {
		// ?
	} else {
		// Buffer size was increased.
		// Still, the contained string has not changed its length!
	}

	(*dest)->buffer[newsize] = '\0';
}

bool nstr_append (nstr* dest, const nstr* src, size_t maxdestsize) {
	if (!dest)
		// cannot append anything to a nullptr
		return false;
	if (!src)
		// okay, nop
		return true;

	const size_t newsize = dest->length + src->length;
	if (newsize > maxdestsize)
		// too long!
		return false;

	memcpy(dest->buffer + dest->length, src->buffer, src->length + 1);
	dest->length = newsize;

	return true;
}

bool nstr_append_a (nstr* *dest, const nstr* src, size_t *maxdestsize) {
	if (!dest)
		// no ptr
		return false;
	if (!src || !src->length)
		// okay, nop
		return true;

	const bool   initial = (*dest == NULL);
	const size_t cursize = (initial) ? 0 : (*dest)->length;
	const size_t newsize = cursize + src->length;

	if (initial || newsize > *maxdestsize) {
		// resize
		realloc_nstr(*dest, newsize + append_reserve);
		*maxdestsize = newsize + append_reserve;
	}

	memcpy((*dest)->buffer + cursize, src->buffer, src->length + 1);
	(*dest)->length = newsize;

	return true;
}

bool nstr_appendc (nstr* dest, char c, size_t maxdestsize) {
	if (!dest)
		// cannot append anything to a nullptr
		return false;

	const size_t newsize = dest->length + 1;
	if (newsize > maxdestsize)
		// too long!
		return false;

	dest->buffer[dest->length++] = c;
	dest->buffer[dest->length  ] = '\0';

	return true;
}

bool nstr_appendc_a (nstr* *dest, char c, size_t *maxdestsize) {
	if (!dest)
		return false;

	const bool   initial = (*dest == NULL);
	const size_t cursize = (initial) ? 0 : (*dest)->length;
	const size_t newsize = cursize + 1;

	if (initial || newsize > *maxdestsize) {
		// resize!
		realloc_nstr(*dest, newsize + append_reserve);
		*maxdestsize = newsize + append_reserve;
	}

	if (initial) {
		(*dest)->length = 1;
		(*dest)->buffer[0] = c;
		(*dest)->buffer[1] = '\0';
	} else {
		(*dest)->length = newsize;
		(*dest)->buffer[cursize] = c;
		(*dest)->buffer[newsize] = '\0';
	}

	return true;
}

bool nstr_appendsz (nstr* dest, const char* src, size_t maxdestsize) {
	if (!dest)
		// cannot append anything to a nullptr
		return false;
	if (!src || !src[0])
		// okay, nop
		return true;

	const size_t srclen = strlen(src);
	const size_t newsize = dest->length + srclen;
	if (newsize > maxdestsize)
		// too long!
		return false;

	memcpy(dest->buffer + dest->length, src, srclen + 1);
	dest->length = newsize;

	return true;
}

bool nstr_appendsz_a (nstr* *dest, const char* src, size_t *maxdestsize) {
	if (!dest)
		return false;
	if (!src || !src[0])
		// nop
		return true;

	const bool   initial = (*dest == NULL);
	const size_t cursize = (initial) ? 0 : (*dest)->length;
	const size_t srclen  = strlen(src);
	const size_t newsize = cursize + srclen;

	if (initial || newsize > *maxdestsize) {
		// resize!
		realloc_nstr(*dest, newsize + append_reserve);
		*maxdestsize = newsize + append_reserve;
	}

	memcpy((*dest)->buffer + cursize, src, srclen + 1);
	(*dest)->length = newsize;

	return true;
}

nstr* nstr_dup (const nstr* src) {
	if (!src)
		return NULL;

	const size_t slen = sizeof(size_t) + src->length + 1;
	return memcpy(Malloc(slen), src, slen);
}

nstr* nstr_fromsz (const char* src) {
	if (!src)
		return NULL;

	const size_t len  = strlen(src);
	const size_t slen = sizeof(size_t) + len + 1;

	nstr* n = Malloc(slen);

	n->length = len;
	memcpy(n->buffer, src, len + 1);

	return n;
}

nstr* nstr_froms (const char* src, size_t length) {
	if (!src)
		return NULL;

	const size_t slen = sizeof(size_t) + length + 1;

	nstr* n = Malloc(slen);

	n->length = length;

	if (length > 0)
		memcpy(n->buffer, src, length);

	// Always add an additional trailing NUL byte:
	n->buffer[length] = '\0';

	return n;
}

bool nstr_cmp (const nstr* a, const nstr* b) {
	if (!a || !b)
		// No string to compare.
		// Even if _both_ pointers are NULL, that's still not a match.
		return false;

	if (a == b)
		// Rare special case:
		// nstr equality is reflexive, of course.
		return true;

	if (a->length != b->length)
		// Length mismatch, those nstr cannot be identical.
		return false;

	for (register size_t p = 0; p < a->length; p++)
		if (a->buffer[p] != b->buffer[p])
			// Byte mismatch.
			return false;

	// Okay, both nstr are of the same length and have the same byte-for-byte content.
	return true;
}

bool nstr_cmpsz (const nstr* a, const char* b) {
	if (!a || !b)
		// No string to compare.
		// Even if _both_ pointers are NULL, that's still not a match.
		return false;

	if (a->buffer == b)
		// Rare special case:
		// if the char* string points to the nstr buffer,
		// then of course the strings are equal.
		return true;
	
	// Since 'b' is a NUL-terminated string,
	// we can just walk it until the first NUL occurs:
	
	const char* aa = a->buffer;
	while (*b) {
		if (*(aa++) != *(b++))
			// Byte mismatch.
			return false;
	}

	// Strings match up until b's final NUL byte.
	// But is this the end of a as well?
	
	return (*aa == '\0' && (size_t)(aa - a->buffer) == a->length);
}

bool nstr_icmpsz (const nstr* a, const char* b) {
	if (!a || !b)
		// No string to compare.
		// Even if _both_ pointers are NULL, that's still not a match.
		return false;

	if (a->buffer == b)
		// Rare special case:
		// if the char* string points to the nstr buffer,
		// then of course the strings are equal.
		return true;
	
	// Since 'b' is a NUL-terminated string,
	// we can just walk it until the first NUL occurs:
	
	const char* aa = a->buffer;
	while (*b) {
		if (tolower(*(aa++)) != tolower(*(b++)))
			// Byte mismatch.
			return false;
	}

	// Strings match up until b's final NUL byte.
	// But is this the end of a as well?
	
	return (*aa == '\0' && (size_t)(aa - a->buffer) == a->length);
}


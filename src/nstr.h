#ifndef NSTR_H
#define NSTR_H
#include <stdbool.h>
#include <stdlib.h>


/**
 * nstr are a NUL-safe string structure.
 * Every nstr contains a length indicator
 * and a char array with the actual string content.
 * If an nstr structure gets duplicated,
 * the contained string will be duplicated as well --
 * the structure does not contain any pointers.
 *
 * The contained string may contain NUL bytes.
 * The contained string must still be NUL-terminated.
 * All nstr functions which create, build, or alter an nstr
 * always make sure that the contained string is NUL-terminated.
 * This trailing NUL byte is _not_ counted in 'length'.
 *
 * This means that nstr_fromsz("") returns a pointer
 * to a { .length=0, .buffer={'\0'} } structure.
 *
 * The char[] buffer in an nstr can be modified directly.
 * Usually, its size is exactly sufficient to accommodate length+1 bytes
 * (the string's length plus the trailing NUL byte),
 * which makes writing past the buffer's boundary inadvisable.
 * One exception to this are nstr structures returned by nstr_init(size),
 * which have length=0 but can accommodate size+1 bytes.
 *
 * Existing nstr can be grown with nstr_resize(), which dynamically reallocates the nstr pointer,
 * or with the nstr_append*_a() function family which also appends another string to the resized buffer.
 *
 * nstr structures are created using plain malloc(3)/realloc(3)
 * and can therefore be destroyed with free(3).
 * Alternatively, nstr_destroy() does the same.
 */
typedef struct nstr {
	size_t length;
	char   buffer [1];
} nstr;


/**
 * Returns a new nstr structure with .length=0
 * and an uninitialized .buffer which can hold 'size' bytes (plus one NUL byte).
 * The first buffer byte will be set to NUL.
 */
nstr* nstr_init (size_t size);

/**
 * Reallocates an nstr structure so that it can hold 'newsize' payload bytes
 * (plus one trailing NUL byte).  The last byte of the reallocated buffer will be set to NUL.
 * If this function is used to grow an existing nstr,
 * the memory between the earlier NUL and the new trailing NUL will be uninitialized.  */
void nstr_resize (nstr* *dest, size_t newsize);

/**
 * Appends the 'src' string onto 'dest', assuming there's enough space left.
 * If dest's current length plus the src's length combined would be greater than 'maxdestsize',
 * the function returns FALSE and does nothing.
 */
bool nstr_append (nstr* dest, const nstr* src, size_t maxdestsize);

/**
 * Appends the 'src' string onto *dest, resizing *dest if there's not enough space left.
 * The buffer's current size must be passed in *maxdestsize.
 * That value will be changed to the new buffer size if the buffer gets resized.
 */
bool nstr_append_a (nstr* *dest, const nstr* src, size_t *maxdestsize);

/**
 * Appends a single character onto 'dest'.
 * If dest's current length plus 1 would be greater than 'maxdestsize',
 * the function returns FALSE and does nothing.
 */
bool nstr_appendc (nstr* dest, char src, size_t maxdestsize);

/**
 * Appends a single character onto 'dest', resizing the buffer if necessary.
 * The buffer's current size must be passed in *maxdestsize;
 * That value will be changed to the new buffer size if the buffer gets resized.
 */
bool nstr_appendc_a (nstr* *dest, char src, size_t *maxdestsize);

/**
 * Appends the 'src' string onto 'dest', assuming there's enough space left.
 * If dest's current length plus strlen(src) combined would be greater than 'maxdestsize',
 * the function returns FALSE and does nothing.
 */
bool nstr_appendsz (nstr* dest, const char* src, size_t maxdestsize);

/**
 * Appends the 'src' string onto 'dest', resizing the 'dest' buffer if necessary.
 * The buffer's current size must be passed in *maxdestsize;
 * That value will be changed to the new buffer size if the buffer gets resized.
 */
bool nstr_appendsz_a (nstr* *dest, const char* src, size_t *maxdestsize);

/**
 * Completely duplicates an existing nstr structure
 * and returns a pointer to the copy.
 */
nstr* nstr_dup (const nstr* src);

/**
 * Truncates an existing nstr structure.
 * The contained string will be reset to "" and its length counter will be reset to zero.
 * Its buffer will NOT be resized/freed.
 */
void nstr_truncate (nstr* s);

/**
 * Destroys an nstr structure.
 * The 's' pointer should not be used in any way after this call.
 */
void nstr_destroy (nstr* s);

/**
 * Creates a new nstr structure from a NUL-terminated string,
 * which will be copied into the new structure.
 */
nstr* nstr_fromsz (const char* src);

/**
 * Creates a new nstr structure from a possibly NUL-terminated string,
 * which will be copied into the new structure.
 * The function will read exactly 'length' bytes from 'src'
 * and add a trailing NUL byte.
 */
nstr* nstr_froms (const char* src, size_t length);

/**
 * Compares two nstr structures.
 * Returns true iff they have the same length and the same byte-for-byte content.
 *
 * If a or b are NULL, the function always returns FALSE,
 * even if both a and b are NULL.
 */
bool nstr_cmp (const nstr* a, const nstr* b);

/**
 * Compares an nstr to a plain NUL-terminated string.
 * Returns true iff a is equal to b (up until b's first NUL byte).
 *
 * If a or b are NULL, the function always returns FALSE,
 * even if both a and b are NULL.
 */
bool nstr_cmpsz (const nstr* a, const char* b);

/**
 * Case-insensitive comparison of an nstr to a plain NUL-terminated string.
 * Returns true iff a is equal to b (up until b's first NUL byte).
 * Uses tolower(3) on every character prior to comparison.
 *
 * If a or b are NULL, the function always returns FALSE,
 * even if both a and b are NULL.
 */
bool nstr_icmpsz (const nstr* a, const char* b);

/**
 * Iterates over an nstr's buffer array,
 * assigning each contained char to a loop-local (char*)'cvar' variable.
 * The final NUL byte won't be processed.
 */
#define for_nstr(nstrp, cvar) \
	for (char* cvar = (nstrp)->buffer; cvar < (nstrp)->buffer + (nstrp)->length; cvar++)


#endif  // NSTR_H

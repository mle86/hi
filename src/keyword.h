#ifndef _KEYWORD_H
#define _KEYWORD_H

/**
 * Manages an internal list of registered KEYWORDs
 * which can later be matched against any input line.
 */

#include <stddef.h>
#include <stdbool.h>


typedef struct keyword {
	const char* word;

	// strlen(word)
	size_t length;

	// Whether this word should be matched case-sensitive or not.
	bool case_sensitive;
} Keyword;

typedef struct keyword_match {
	const Keyword* keyword;
	size_t position;
} KeywordMatch;


/**
 * Registers one keyword for matching.
 * Assumes that 'word' is a permanently-valid str pointer that won't be freed or modified by the caller; add_keyword may modify it.
 */
const Keyword* add_keyword (char* word, bool case_sensitive);

/** Returns the number of keywords already registered with add_keyword(). */
size_t n_keywords (void);

/**
 * Returns the first keyword match found in the input string, or NULL if none was found.
 * The returned pointer is only valid until the next call!
 */
const KeywordMatch* find_first_keyword (const char* input);


#endif  // _KEYWORD_H

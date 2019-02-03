/*
 * This file is part of the 'hi' project
 * (see https://github.com/mle86/hi).
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2007-2019 Maximilian Eul
 */

#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <err.h>
#include "keyword.h"
#include "exit.h"
#include "limits.h"


static size_t nkw = 0;
static Keyword keywords [MAX_KEYWORDS] = {NULL};

static bool match_word (const char* input, const Keyword* k);


// Registers one keyword for matching.
const Keyword* add_keyword (char* word, bool case_sensitive) {
	if (!word || !word[0])
		return NULL;

	if (nkw >= MAX_KEYWORDS - 1)
		errx(EXIT_TOO_MANY_KEYWORDS, "too many keywords");

	Keyword* kw = &keywords[nkw++];
	kw->length         = strlen(word);
	kw->case_sensitive = case_sensitive;
	kw->word           = word;

	return kw;
}

// Returns the number of keywords already registered with add_keyword().
size_t n_keywords (void) {
	return nkw;
}

// Returns the first keyword match found in the input string, or NULL if none was found.
const KeywordMatch* find_first_keyword (const char* input) {
	// TODO: This is really inefficient. Maybe use a trie?
	for (size_t p = 0; input[p]; p++) {
		for (size_t k = 0; k < nkw; k++) {
			if (match_word(&input[p], &keywords[k])) {
				static KeywordMatch match = { 0 };
				match.keyword  = &keywords[k];
				match.position = p;
				return &match;
			}
		}
	}

	return NULL;
}

// True if 'input' starts with 'k->word'.
inline bool match_word (const char* input, const Keyword* k) {
	if (k->case_sensitive) {
		return (memcmp(input, k->word, k->length) == 0);
	} else {
		return (strncasecmp(input, k->word, k->length) == 0);
	}
}


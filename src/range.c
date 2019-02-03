/*
 * This file is part of the 'hi' project
 * (see https://github.com/mle86/hi).
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2007-2019 Maximilian Eul
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>
#include <err.h>
#include "exit.h"
#include "range.h"
#include "limits.h"


// Number of registered line ranges to match.
static size_t nrg = 0;

// Ranges to match. This list is sorted by init_ranges().
static Range ranges [MAX_RANGES] = { 0 };

// Points to the currently-matching range or, if not matching, to the next range.
// NULL if no ranges remain (or prior to init_ranges() call).
static const Range* current_range = NULL;

static int  cmp_ranges (const void* a, const void* b);
static void next_range (size_t cur_lineno);
static bool parse_int  (const char* input, char* *endptr, size_t *result);


// Returns the number of line ranges previously registered with range_add().
size_t n_ranges (void) {
	return nrg;
}

// Registers a new line range to highlight.
const Range* add_range (size_t start, size_t end) {
	if (start <= 0 || end <= 0)
		return NULL;

	if (nrg >= MAX_RANGES - 1)
		errx(EXIT_TOO_MANY_RANGES, "too many ranges");

	if (start > end) {
		// swap:
		const size_t t = start;
		start = end;
		end   = t;
	}

	Range* rg = &ranges[nrg++];
	rg->start = start;
	rg->end   = end;
	return rg;
}

const Range* add_raw_range (const char* input) {
	if (!input || !input[0])
		return NULL;

	#define SKIP_WHITESPACE(cp)  while(*(cp)==' ' || *(cp)=='\t' || *(cp)=='\r' || *(cp)=='\n')  (cp)++
	SKIP_WHITESPACE(input);

	size_t start;
	const char* s = input;
	if (!parse_int(s, (char**)&s, &start) || start <= 0)
		// invalid start number
		return NULL;

	size_t end = start;
	if (*s == '-') {
		// there's more!
		if (!parse_int(++s, (char**)&s, &end) || end <= 0)
			// invalid end number
			return NULL;
	}

	return add_range(start, end);
}

// Should be called once after adding all ranges using add_range() and before using is_in_range() for the first time.
void init_ranges (void) {
	qsort(ranges, nrg, sizeof(Range), cmp_ranges);
	current_range = (nrg > 0) ? &ranges[0] : NULL;
}

// True iff 'lineno' is in one of the previously-registered ranges.
RangeResult is_in_range (size_t lineno) {
	if (!current_range)
		// No active ranges remaining. Abort.
		return R_ENDED;
	if (lineno < current_range->start)
		// The next range has not yet been reached.
		return R_NO_MATCH;
	if (lineno > current_range->end) {
		// The current range has been passed. Find the next one:
		next_range(lineno);
		return is_in_range(lineno);
	}
	
	// The lineno is within a registered range!
	return R_MATCH;
}


int cmp_ranges (const void* a, const void* b) {
	const Range* ra = a;
	const Range* rb = b;

	if (ra->start < rb->start)
		return -1;
	if (ra->start > rb->start)
		return +1;
	return 0;
}

void next_range (size_t cur_lineno) {
	if (!current_range)
		// No active ranges remaining. Abort.
		return;

	do {
		current_range++;

		if ((size_t)(current_range - &ranges[0]) >= nrg) {
			// out of bounds, no ranges remaining
			current_range = NULL;
			return;
		}

	} while (current_range->end < cur_lineno);
	// do...while guarantees we increase current_range at least once.
	// The while condition _repeats_ that in case the next range has already been passed too.
}

bool parse_int (const char* input, char* *endptr, size_t *result) {
	if (!input || !input[0])
		return false;

	errno = 0;
	char* e = NULL;
	uintmax_t n = strtoumax(input, &e, 10);

	if (errno == ERANGE)
		// overflow
		return false;

	if (n == 0 && e == input)
		// no digits in input
		return false;

	// Success!
	if (endptr)
		*endptr = e;
	*result = (size_t)n;
	return true;
}


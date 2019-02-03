/**
 * Manages an internal list of registered Ranges of line numbers
 * which should be highlighted.
 *
 * This file is part of the 'hi' project
 * (see https://github.com/mle86/hi).
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2007-2019 Maximilian Eul
 */

#ifndef _RANGE_H
#define _RANGE_H

#include <stddef.h>


typedef struct range {
	size_t start;  // first line to highlight
	size_t end;  // last line to highlight (inclusive)
} Range;

typedef enum range_check_result {
	// The checked line is not in one of the registered ranges.
	R_NO_MATCH,

	// The checked line is in one of the ranges!
	R_MATCH,

	// The checked line is not in one of the registered ranges,
	// and there are no more remaining ranges for higher line numbers.
	R_ENDED,
} RangeResult;


/**
 * Returns the number of line ranges previously registered with range_add().
 */
size_t n_ranges (void);

/**
 * Registers a new line range to highlight.
 */
const Range* add_range (size_t start, size_t end);

/**
 * Registers a new line range to highlight,
 * parsing the input string for a range definition.
 * Returns NULL if the input could not be parsed.
 */
const Range* add_raw_range (const char* input);

/**
 * Should be called once after adding all ranges using add_range() and before using is_in_range() for the first time.
 * This sorts the stored ranges.
 */
void init_ranges (void);

/**
 * Determines if 'lineno' is in one of the previously-registered line ranges.
 * Should not be used before finalize_ranges() has been called once.
 * This function assumes that it will not be called repeatedly with the same argument.
 * This function assumes that it will only be called with an argument greater than the last call's argument.
 */
RangeResult is_in_range (size_t lineno);


#endif  // _RANGE_H

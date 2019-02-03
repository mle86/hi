/**
 * Constants to select the main program operation mode.
 *
 * This file is part of the 'hi' project
 * (see https://github.com/mle86/hi).
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2007-2019 Maximilian Eul
 */

#ifndef _MODE_H
#define _MODE_H


typedef enum mode {
	MODE_LINE,
	MODE_PARAGRAPH,
	MODE_WORDS,
	MODE_EXPLICIT,

	DEFAULT_MODE = MODE_LINE,
} Mode;


#endif  // _MODE_H

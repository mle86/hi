/**
 * Exit status constants.
 *
 * This file is part of the 'hi' project
 * (see https://github.com/mle86/hi).
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2007-2019 Maximilian Eul
 */

#ifndef _EXIT_H
#define _EXIT_H


#define EXIT_HELP 0

#define EXIT_SYNTAX 1
#define EXIT_NO_KEYWORDS EXIT_SYNTAX
#define EXIT_NO_RANGES EXIT_SYNTAX

#define EXIT_TOO_MANY_INPUTS 2
#define EXIT_TOO_MANY_RANGES EXIT_TOO_MANY_INPUTS
#define EXIT_TOO_MANY_KEYWORDS EXIT_TOO_MANY_INPUTS

#define EXIT_OUT_OF_MEMORY 3


#endif  // _EXIT_H

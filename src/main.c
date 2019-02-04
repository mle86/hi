/*
 * This file is part of the 'hi' project
 * (see https://github.com/mle86/hi).
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2007-2019 Maximilian Eul
 */

#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include "keyword.h"
#include "range.h"
#include "limits.h"
#include "color.h"
#include "nstr.h"
#include "mode.h"
#include "exit.h"

#define VERSION "2.1.0"

#define streq(a, conststr) (0 == strncmp((a), conststr, sizeof(conststr)))

static void Help (void);
static void Version (void);
static void main_lines (Color highlight_color);
static void main_words (Color highlight_color);
static void main_paragraphs (Color highlight_color);
static void main_explicit (Color highlight_color);
static bool is_paragraph_separator (const char* line);


int main (int argc, char** argv) {

	// Defaults:  //////////////////////////////////////////////////////////

	Mode  mode            = DEFAULT_MODE;
	Color highlight_color = DEFAULT_COLOR;
	bool  case_sensitive  = false;


	// Options:  ///////////////////////////////////////////////////////////


	const char* options = "wplL:c:iIhV";
	const struct option long_options [] = {
		{ "words",          no_argument,       NULL, 'w' },
		{ "lines",          no_argument,       NULL, 'l' },
		{ "paragraphs",     no_argument,       NULL, 'p' },
		{ "explicit",       required_argument, NULL, 'L' },
		{ "ranges",         required_argument, NULL, 'L' },

		{ "help",           no_argument,       NULL, 'h' },
		{ "version",        no_argument,       NULL, 'V' },

		{ "ignore-case",    no_argument,       NULL, 'i' },
		{ "case-sensitive", no_argument,       NULL, 'I' },
		{ "color",          required_argument, NULL, 'c' },
		{ "colour",         required_argument, NULL, 'c' },

		{ 0, 0, 0, 0 }};

	int c;
	while ((c = getopt_long(argc, argv, options, long_options, NULL)) != -1)
	switch (c) {
		case 'w':  mode = MODE_WORDS; break;
		case 'l':  mode = MODE_LINE; break;
		case 'p':  mode = MODE_PARAGRAPH; break;
		case 'i':  case_sensitive = false; break;
		case 'I':  case_sensitive = true; break;
		case 'c':
			if (!identify_color(optarg, &highlight_color)) {
				errx(EXIT_SYNTAX, "unknown color: %s", optarg);
			}
			break;
		case 'L':
			mode = MODE_EXPLICIT;
			if (optarg[0] == '-') {
				// Doesn't look like a range. Retry as option.
				optind--;
			} else if (!add_raw_range(optarg)) {
				errx(EXIT_SYNTAX, "invalid range: %s", optarg);
			}
			break;
		case 'h': Help(); break;
		case 'V': Version(); break;
	}


	// Arguments:  /////////////////////////////////////////////////////////

	while (optind < argc) {
		const char* arg = argv[optind++];

		if (mode == MODE_EXPLICIT) {
			// In explicit mode (-L), all non-option arguments are line ranges:
			if (!add_raw_range(arg)) {
				errx(EXIT_SYNTAX, "invalid range: %s", arg);
			}
		} else {
			// In all other modes (-wpl), all non-option arguments are KEYWORDs:
			add_keyword(strdup(arg), case_sensitive);
		}
	}


	// Pre-Take-Off Checks:  ///////////////////////////////////////////////

	if (mode != MODE_EXPLICIT && !n_keywords()) {
		errx(EXIT_NO_KEYWORDS, "no keywords");
	}
	if (mode == MODE_EXPLICIT && !n_ranges()) {
		errx(EXIT_NO_RANGES, "no ranges");
	}


	// Mode Switch:  ///////////////////////////////////////////////////////

	switch (mode) {
		case MODE_WORDS: main_words(highlight_color); break;
		case MODE_LINE: main_lines(highlight_color); break;
		case MODE_PARAGRAPH: main_paragraphs(highlight_color); break;
		case MODE_EXPLICIT: main_explicit(highlight_color); break;
	}

	return 0;
}


// Main Work Functions:  ///////////////////////////////////////////////////////

void main_lines (Color highlight_color) {
	char line [MAX_LINELEN];
	while (fgets(line, MAX_LINELEN, stdin)) {
		if (find_first_keyword(line)) {
			colorize(highlight_color, stdout, line);
		} else {
			fputs(line, stdout);
		}
	}
}

void main_words (Color highlight_color) {
	char line [MAX_LINELEN];
	while (fgets(line, MAX_LINELEN, stdin)) {
		const KeywordMatch* match;
		const char* p = line;
		while ((match = find_first_keyword(p))) {
			// Print line part prior to match in regular color:
			printf("%.*s", (int)match->position, p);
			// Print the match in highlighted color:
			colorizef(highlight_color, stdout, "%.*s", (int)match->keyword->length, p + match->position);
			// and advance the string to continue past the match:
			p += match->position + match->keyword->length;
		}

		// No more matches in the rest of the line
		// (or possibly no match in the entire line).
		// Print the rest:
		fputs(p, stdout);
	}
}

void main_paragraphs (Color highlight_color) {
	char line [MAX_LINELEN];
	size_t size = MAX_LINELEN * 4;
	nstr* para = nstr_init(size);
	bool matched = false;

	while (fgets(line, MAX_LINELEN, stdin)) {
		if (is_paragraph_separator(line)) {
			// current paragraph ended!
			if (matched) {
				colorize(highlight_color, stdout, para->buffer);
				colorize(highlight_color, stdout, line);
			} else {
				fputs(para->buffer, stdout);
				fputs(line, stdout);
			}
			matched = false;
			nstr_truncate(para);

		} else if (matched) {
			// We already know the current paragraph is a match.
			// No need to buffer it.
			colorize(highlight_color, stdout, line);

		} else if (find_first_keyword(line)) {
			// The line is the first matching line in this paragraph!
			// Highlight everything we got so far and set the flag for more highlighting:
			colorize(highlight_color, stdout, para->buffer);
			colorize(highlight_color, stdout, line);
			matched = true;
			nstr_truncate(para);

		} else {
			// Add the line to the paragraph buffer.
			// It might yet turn out to contain a keyword...
			nstr_appendsz_a(&para, line, &size);
		}
	}

	// Reading ended but we might still be in the middle of a paragraph.
	// We need to print it, possibly more highlighted:
	if (matched) {
		colorize(highlight_color, stdout, para->buffer);
	} else {
		fputs(para->buffer, stdout);
	}
}

void main_explicit (Color highlight_color) {
	init_ranges();

	char line [MAX_LINELEN];
	size_t lineno = 1;
	while (fgets(line, MAX_LINELEN, stdin)) {
		switch (is_in_range(lineno++)) {
			case R_MATCH:
				// Range match!
				colorize(highlight_color, stdout, line);
				break;
			case R_NO_MATCH:
				// No match, just print the line as-is.
				// Stay in the while loop, there are remaining ranges.
				fputs(line, stdout);
				break;
			case R_ENDED:
				// No match and there will be no more matches.
				fputs(line, stdout);
				// Leave this while loop for faster as-is printing of the rest:
				goto rest;
		}
	}

  rest:
	// The rest of the input cannot match any ranges anymore.
	// Just dump everything as-is:
	if (feof(stdin))
		return;

	while (fgets(line, MAX_LINELEN, stdin)) {
		fputs(line, stdout);
	}
}


// Helper Functions:  //////////////////////////////////////////////////////////

#define SKIP_WHITESPACE(cp)  while(*(cp)==' ' || *(cp)=='\t' || *(cp)=='\r' || *(cp)=='\n')  (cp)++

bool is_paragraph_separator (const char* line) {
	if (!line)
		return true;
	SKIP_WHITESPACE(line);
	if (!line[0])
		return true;
	return false;
}

void Help (void) {
	// abbreviations:
	#define M1 COLOR_BOLD
	#define M0 COLOR_SGR0
	#define Mu COLOR_UNDERLINED

	#define KW Mu "KEYWORD" M0
	#define RG Mu "RANGE" M0

	printf(
		M1 PROGNAME M0 " reads text from standard input and highlights\n"
		"those paragraphs/lines containing at least one of the given keywords.\n"
		"Usage: " M1 PROGNAME M0 " [" M1 "-w" M0 "|" M1 "-p" M0 "|" M1 "-l" M0 "] [" M1 "OPTIONS" M0 "] " KW "...\n"
		"Usage: " M1 PROGNAME M0 "  " M1 "-L" M0 "       "                      " [" M1 "OPTIONS" M0 "] " RG "...\n"
		"Options:\n"
		"  " M1 "-p" M0 "         Highlight paragraphs containing at least one of the " KW "s.\n"
		"  " M1 "-l" M0 "         Highlight lines containing at least one of the " KW "s.\n"
		"  " M1 "-w" M0 "         Highlight only " KW "s.\n"
		"  " M1 "-L" M0 " [" RG "] Highlight only the given line(s).\n"
		"             RANGE: either a single line number "
		                             "(e.g. "Mu "71"M0") "
		                             "or a range (e.g. " Mu "1-9" M0 ").\n"
		"  " M1 "-c" M0 " " Mu "COLOR" M0 "   Select highlighting color, choose from\n"
		"             " Mu "white" M0 ", " Mu "red" M0 ", " Mu "green" M0 ", "
		               Mu "blue" M0 ", " Mu "yellow" M0 ", " Mu "cyan" M0 ".\n"
		"  " M1 "-i" M0 "         Case-insensitive matching (default).\n"
		"  " M1 "-I" M0 "         Case-sensitive matching.\n"
		"  " M1 "-h" M0 "         This help.\n"
		"  " M1 "-V" M0 "         Program version information.\n"
		"Options "M1"-plwL"M0" are mutually exclusive.\n"
		"Defaults are "M1"-l -c yellow"M0".\n"
		"\n"
	);
	exit(EXIT_HELP);
}


void Version (void) {
	printf(
		"%s v%s\n"
		"Written by Maximilian Eul <maximilian@eul.cc>, January 2019.\n"
		"\n"
		, PROGNAME, VERSION
	);
	exit(EXIT_HELP);
}


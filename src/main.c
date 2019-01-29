#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "keyword.h"
#include "limits.h"
#include "color.h"
#include "nstr.h"
#include "mode.h"
#include "exit.h"

#define VERSION "2.1.0"

static void Help (void);
static void Version (void);
static void main_lines (Color highlight_color);
static void main_words (Color highlight_color);
static void main_paragraphs (Color highlight_color);
static bool is_paragraph_separator (const char* line);


int main (int argc, char** argv) {

	// Defaults:  //////////////////////////////////////////////////////////

	Mode  mode            = DEFAULT_MODE;
	Color highlight_color = DEFAULT_COLOR;
	bool  case_sensitive  = false;


	// Options:  ///////////////////////////////////////////////////////////

	signed char c;
	while ((c = getopt(argc, argv, "wplL:c:hV")) != -1)
	switch (c) {
		case 'w':  mode = MODE_WORDS; break;
		case 'l':  mode = MODE_LINE; break;
		case 'p':  mode = MODE_PARAGRAPH; break;
		case 'i':  case_sensitive = false; break;
		case 'I':  case_sensitive = true; break;
		case 'c':
			if (!identify_color(optarg, &highlight_color)) {
				die(EXIT_SYNTAX, "unknown color: %s", optarg);
			}
			break;
		case 'L':
			mode = MODE_EXPLICIT;
			break;
		case 'h': Help(); break;
		case 'V': Version(); break;
	}


	// Arguments:  /////////////////////////////////////////////////////////

	while (optind < argc) {
		add_keyword(strdup(argv[optind]), case_sensitive);
		optind++;
	}


	// Pre-Take-Off Checks:  ///////////////////////////////////////////////

	if (!n_keywords()) {
		die(EXIT_NO_KEYWORDS, "no keywords");
	}


	// Mode Switch:  ///////////////////////////////////////////////////////

	switch (mode) {
		case MODE_WORDS: main_words(highlight_color); break;
		case MODE_LINE: main_lines(highlight_color); break;
		case MODE_PARAGRAPH: main_paragraphs(highlight_color); break;
		// TODO
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
			printf("%s", line);
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
		printf("%s", p);
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
				printf("%s", para->buffer);
				printf("%s", line);
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
		printf("%s", para->buffer);
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

	printf(
		M1 PROGNAME M0 " reads text from standard input and highlights\n"
		"those paragraphs/lines containing at least one of the given keywords.\n"
		"Usage: " M1 PROGNAME M0 " [" M1 "OPTIONS" M0 "] " Mu "KEYWORD" M0 "...\n"
		"Options:\n"
		"  " M1 "-p" M0 "        Highlight paragraphs containing at least one of the "Mu "KEYWORD"M0"s.\n"
		"  " M1 "-l" M0 "        Highlight lines containing at least one of the "Mu "KEYWORD"M0"s.\n"
		"  " M1 "-w" M0 "        Highlight only KEYWORD"M0"s\n"
		"  " M1 "-L" M0 " " Mu "nn" M0"     Highlight only the given line(s), no "Mu "KEYWORD"M0"s needed.\n"
		"            " Mu "nn" M0 ": either a single line number "
		                             "(e.g. "Mu "71"M0") "
		                             "or a range (e.g. " Mu "1-9" M0 ").\n"
		"  " M1 "-c" M0 " " Mu "COLOR" M0 "  Select highlighting color, choose from\n"
		"            " Mu "white" M0 ", " Mu "red" M0 ", " Mu "green" M0 ", "
		               Mu "blue" M0 ", " Mu "yellow" M0 ", " Mu "cyan" M0 ".\n"
		"  " M1 "-h" M0 "        This help.\n"
		"  " M1 "-V" M0 "        Program version information.\n"
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


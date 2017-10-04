#include "aux.h"

/*  Copyright (C) 2010  Maximilian L. Eul
    This file is part of hi.

    hi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    hi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with hi.  If not, see <http://www.gnu.org/licenses/>.
*/

static int _sortwrapper_range (const void* r1, const void* r2);


inline bool line_is_blank (register const char* s) {
	register char c;
	while( (c = *s) && isspace(c) ) {
		s++;
		if (!(c = *s) || !isspace(c)) break; s++;
		if (!(c = *s) || !isspace(c)) break; s++;
		if (!(c = *s) || !isspace(c)) break; s++;
		if (!(c = *s) || !isspace(c)) break; s++;
		if (!(c = *s) || !isspace(c)) break; s++;
		if (!(c = *s) || !isspace(c)) break; s++;
		if (!(c = *s) || !isspace(c)) break; s++;
	}
	return (c ? false : true);
}

bool get_range (register const char* s, Range *r) {
	SKIP_WHITESPACE(s);
	long int v = atoi(s);
	if (v <= 0)  return false;

	r->start = r->stop = v;
	while( isdigit(*s) )  s++;
	if (! *s)  return true;

	if (*s != ':' && *s != '-')  return false;
	v = atoi(s + 1);
	if (v <= 0)  return false;

	r->stop = v;
	return true;
}

void sort_ranges (Range *r, uint cnt) {
	Range* orig   = r;
	uint orig_cnt = cnt;

	// clean up
	while( cnt > 0 && r->start > 0 ) {
		r++;
		cnt--;
	}
	register Range* t = r + 1;
	while( cnt-- > 0 ) {
		while( t->start <= 0 )  t++;
		memcpy(r++, t++, sizeof(*r));
	}

	qsort(orig, orig_cnt, sizeof(Range), _sortwrapper_range);
}

int _sortwrapper_range (const void* _r1, const void* _r2) {
	const Range* r1 = (Range *)_r1;
	const Range* r2 = (Range *)_r2;
	if (!r1->start || r1->start >= r2->stop)
		return 1;
	else if (!r2->start || r1->stop <= r2->start)
		return -1;
	else
		return 0;
}


char* find_keywords (const char* line, const Word* keywords, const uint count, Word* *foundword) {
	char* found = NULL;
	register uint w;
	for( w=0; w<count; w++ )
		if ((found = strstr(line, keywords[w].str)) != NULL)
			break;

	if (foundword != NULL) // save found word?
		*foundword = found ? (Word *)&(keywords[w]) : NULL;

	return found;
}

void output_paragraph (char** lines, ulint *count, const short color) {
	if (!count || !lines || *count<=0)
		return;

	if (color > 0)
		printf("[1;%02hum", color);

	while( (*count)-- > 0 )
		printf("%s", *(lines++));

	*count = 0;
	lines[0] = NULL;
}

void output_line (char* line, const short color) {
	if (!line || !line[0])
		return;

	if (color > 0)
		printf("[1;%02hum", color);
	printf("%s", line);
	if (color > 0)
		printf("[0m");

	line[0] = '\0';
}

short identifyColor (const char* name) {
	if (striequal(name, "gr") || striequal(name, "gn") || striequal(name, "green"))
		return ANSI_GREEN;
	else if (striequal(name, "re") || striequal(name, "rd") || striequal(name, "red"))
		return ANSI_RED;
	else if (striequal(name, "bl") || striequal(name, "blue"))
		return ANSI_BLUE;
	else if (striequal(name, "ye") || striequal(name, "yw") || striequal(name, "yellow"))
		return ANSI_YELLOW;
	else if (striequal(name, "cy") || striequal(name, "cn") || striequal(name, "cyan"))
		return ANSI_CYAN;
	else if (striequal(name, "wh") || striequal(name, "white"))
		return ANSI_WHITE;
	return DEFAULT_COLOR;
}


void Help (void) { printf(
	M1 PROGNAME M0"  reads text  from  standard input  and  highlights\n"
	"those paragraphs/lines containing at least one of the given keywords.\n"
	"Usage: "M1 PROGNAME M0" ["M1"OPTIONS"M0"] "M1"KEYWORD"M0"...\n"
	"Options:\n"
	M1"  -p        "M0"Highlight paragraphs containing at least one of the "M1"KEYWORD"M0"s.\n"
	M1"  -l        "M0"Highlight lines containing at least one of the "M1"KEYWORD"M0"s.\n"
	M1"  -w        "M0"Highlight only KEYWORD"M0"s\n"
	M1"  -L nn     "M0"Highlight only the given line(s), no "M1"KEYWORD"M0"s needed.\n"
	  "            "M1"nn: "M0"either a single line number (e.g. "M1"71"M0") or a range (e.g. "M1"1-9"M0").\n"
	M1"  -c COLOR  "M0"Select highlighting color, choose from\n"
	  "            "  "white, red, green, blue, yellow, cyan.\n"
	M1"  -h        "M0"This help.\n"
	M1"  -V        "M0"Program version information.\n"
	"Options "M1"-plwL"M0" are mutually exclusive.\n"
	"Defaults are "M1"-l -c yellow"M0".\n"
	"\n"
); exit(RET_HELP); }


void Version (void) { printf(
	PROGNAME " v" VERSION "\n"
	"Written by Maximilian Eul <mle@multinion.de>, November 2010.\n"
	"\n"
); exit(RET_HELP); }

#include "hi.h"

/*  Copyright (C) 2019  Maximilian L. Eul
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

int main (int argc, char** argv) {
	short mode = DEFAULT_MODE;
	short color = DEFAULT_COLOR;

	Range ranges [MAXRANGES];
	uint rcnt = 0;

	register signed char c;
	while ((c = getopt(argc, argv, "wplL:c:hV")) != -1)
	switch(c) {
		case 'w': mode = MODE_WORD; break;
		case 'p': mode = MODE_PARA; break;
		case 'l': mode = MODE_LINE; break;
		case 'c': color = identifyColor(optarg); break;
		case 'h': Help(); return RET_HELP;
		case 'V': Version(); return RET_HELP;
		case 'L':
			mode = MODE_EXPLICIT;
			Range r;
			if (rcnt >= MAXRANGES) {
				fprintf(stderr, PROGNAME": too many ranges\n");
				break;
			}
			if (! get_range(optarg, &r)) {
				fprintf(stderr, PROGNAME": invalid range '%s'\n", optarg);
				break;
			}
			addRange(&r, ranges, &rcnt);
			break;
	}
	
	if (mode == MODE_EXPLICIT) {
		sort_ranges(ranges, rcnt);
		explicitLines(color, ranges, rcnt);
		return RET_OK;
	}
	
	uint cnt_words = 0;
	Word keywords [MAXKEYWORDS];

	while (optind < argc)
		if (addWord(argv[optind], keywords, &cnt_words))
			optind++;
		else break;

	if (optind < argc) {
		fprintf(stderr, "%s: too many keywords (max. %i)\n", argv[0], MAXKEYWORDS);
		return RET_TOOMANYKEYWORDS;
	} else if (cnt_words <= 0) {
		fprintf(stderr, "%s: no keywords given\n", argv[0]);
		return RET_NOKEYWORDS;
	}

	if (mode == MODE_PARA)
		scanParagraphs(color, keywords, cnt_words);
	else
		scanLines(color, keywords, cnt_words, mode);

	return RET_OK;
}


bool addWord (char* new, Word* words, uint *count) {
	if (*count >= MAXKEYWORDS || !words)
		return false;
	if (!new || !new[0])
		return true;

	register char* t;
	uint len = 0;
	for (t=new; *t; t++,len++)
		*t = tolower(*t);

	register uint w;
	for (w=0; w<*count; w++)
		if (words[w].length == len && strequal(words[w].str, new))
			// duplicate
			return true;

	words[w].str    = strdup(new);
	words[w].length = len;
	(*count)++;
	return true;
}

void addRange (const Range* _n, Range *r, uint *cnt) {
	Range n = { .start = _n->start, .stop = _n->stop };
	Range* dest = r + *cnt;
	uint rest;

	#define delR() do{ r->start=0; (*cnt)--; }while(0)
	for (rest=*cnt; rest>0; r++) {
		if (! r->start) {
			// found an empty range struct
			dest = r;
			continue;
		}
		rest--;

		bool ia = (r->start <= n.start && n.start <= r->stop),
		     ib = (r->start <= n.stop && n.stop <= r->stop);

		if (ib && n.start < r->start) {
			n.stop = r->stop;
			delR();
		} else if (ia && n.stop > r->stop) {
			n.start = r->start;
			delR();
		} else if (ia && ib)
			return;  // r>n
		else if (n.start < r->start && n.stop > r->stop)
			delR();  // n>r
	}
	#undef delR
	
	dest->start = n.start;
	dest->stop = n.stop;
	(*cnt)++;
}


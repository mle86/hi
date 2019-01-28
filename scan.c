#include "scan.h"

/*  Copyright (C) 2013  Maximilian L. Eul
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

#define freadline(fh,buf) ( !feof(fh) && fgets(buf, sizeof(buf)-1, fh) )

void explicitLines (const short color, const Range* ranges, uint rcnt) {
	ulint line = 0;
	uint r = 0;
	char buf [MAXLINELEN] = "";

	while (r < rcnt && freadline(stdin, buf)) {
		line++;
		while (ranges->stop < line) {
			ranges++;
			if (++r >= rcnt)
				goto dump_all;
		}
		if (ranges->start <= line && ranges->stop >= line)
			output_line(buf, color);
		else
			printf("%s", buf);
	}

	while (freadline(stdin, buf)) {
		dump_all:
		printf("%s", buf);
	}
}

void scanLines (const short color, const Word* keywords, const uint cnt_words, const short mode) {
	char _buf [MAXLINELEN] = "";

	while (freadline(stdin, _buf)) {
		char* found;
		Word* wf;

		char* buf = _buf;
		rest_of_line:

		if (line_is_blank(buf) || !(found = find_keywords(buf, keywords, cnt_words, &wf))) {
			printf("%s", buf);
			continue;
		}

		if (mode == MODE_WORD) {
			int len_prefix = found - buf;
			printf("%.*s[1;%02hum%.*s[0m",
				len_prefix, buf,
				color, (int)wf->length, found
			);
			buf = found + wf->length;
			goto rest_of_line;
		} else {
			output_line(buf, color);
		}

	}
}

void scanParagraphs (const short color, const Word* keywords, const uint cnt_words) {
	char buf [MAXLINELEN];
	bool keyword_in_para = false;

	ulint pl = 0;
	char* paralines [MAXPARALINES] = { NULL };

	while (freadline(stdin, buf)) {

		if (line_is_blank(buf)) {
			// paragraph ended
			output_paragraph(paralines, &pl, keyword_in_para ? color : 0);
			printf(M0"%s", buf);
			keyword_in_para = false;
			continue;
		} else if (keyword_in_para) {
			printf("%s", buf);
			continue;
		}  // else: regular line

		if (find_keywords(buf, keywords, cnt_words, NULL)) {
			// yes, print saved lines
			output_paragraph(paralines, &pl, color);
			keyword_in_para = true;
		} else {
			// no -- save line
			if (pl >= MAXPARALINES)
				output_paragraph(paralines, &pl, 0);
			paralines[pl++] = strdup(buf);
		}
	}

	if (pl > 0)
		output_paragraph(paralines, &pl, keyword_in_para ? color : 0);
}


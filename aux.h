#ifndef __AUX_H
#define __AUX_H
#include "hi.h"

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

bool  get_range (const char* s, Range *r);
void  sort_ranges (Range *r, uint cnt);
char* find_keywords (const char* line, const Word* keywords, const uint count, Word* *foundword);

inline bool line_is_blank (const char* s);

void output_paragraph (char** lines, ulint *count, const short color);
void output_line      (char* line,                 const short color);

short identifyColor (const char* name);

void Help    (void);
void Version (void);


#endif /* __AUX_H */

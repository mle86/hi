#ifndef _SCAN_H
#define _SCAN_H
#include "hi.h"

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


void explicitLines  (const short color, const Range* ranges, uint rcnt);
void scanLines      (const short color, const Word* keywords, const uint cnt_words, const short mode);
void scanParagraphs (const short color, const Word* keywords, const uint cnt_words);


#endif  // _SCAN_H

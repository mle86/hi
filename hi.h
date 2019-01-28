#ifndef _HI_H
#define _HI_H

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

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "const.h"
#include "macros.h"
#include "types.h"
#include "aux.h"
#include "scan.h"


int main (int argc, char** argv);

bool addWord (char* new, Word *words, uint *count);

void addRange (const Range* _n, Range *r, uint *cnt);
void cleanRanges (Range *r, uint cnt);


#endif  // _HI_H

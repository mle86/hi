#ifndef _CONST_H
#define _CONST_H
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


#define VERSION  "2.01"

#define MAXKEYWORDS	  32
#define MAXLINELEN	8192
#define MAXPARALINES	4096
#define MAXRANGES	  64

enum {
	MODE_WORD,
	MODE_PARA,
	MODE_LINE,
	MODE_EXPLICIT,

	DEFAULT_MODE = MODE_LINE,
};

#define DEFAULT_COLOR ANSI_YELLOW
#define ANSI_RED     31
#define ANSI_GREEN   32
#define ANSI_YELLOW  33
#define ANSI_BLUE    34
#define ANSI_WHITE   37
#define ANSI_CYAN    36

#define M1 "[1m"
#define M0 "[0m"


#define RET_OK			0
#define RET_HELP		0

#define RET_TOOMANYKEYWORDS	1
#define RET_NOKEYWORDS		2
#define RET_READERROR		3
#define RET_NOLINES		4


#endif  // _CONST_H

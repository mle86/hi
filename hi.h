#ifndef __HI_H
#define __HI_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "str.h"
#include "const.h"

#include "str.c"


int main (int argc, char** argv);
void init (void);
int ExplicitOnly (char* argv0, char* lines, int color);

short ScanLine (char** Words, uint wordcnt, char* line, uint len);
void PrintParagraph (char** Lines, uint linecnt, short color, short brk);

void SetColor (char* arg, short* c);
char* Prepare (char* raw, uint len);

void Help (void);
void Version (void);


#endif // __HI_H

#ifndef __HI_H
#define __HI_H

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

#include "str.h"
#include "const.h"
#include "typedef.h"

#include "str.c"


int main (int argc, char** argv);
void init (void);
int ExplicitOnly (char* argv0, char* lines, int color);

bool ScanLine (char** Words, uint wordcnt, char* rawline, ulint len,  char* *savePos, ulint *saveWlen);
void PrintParagraph (char** Lines, uint linecnt, short color, bool brk);

void SetColor (char* arg, short* c);
char* Prepare (char* raw, ulint len);

void Help (void);
void Version (void);


#endif // __HI_H

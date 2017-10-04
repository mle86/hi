#ifndef __MACROS_H
#define __MACROS_H
#include <string.h>

char _mhc;

#define IsWhitespace(c)     ( (_mhc=c), ((_mhc == ' ') || (_mhc == '\t')) )
#define IsExtWhitespace(c) \
	( (_mhc=c), (IsWhitespace(_mhc) || (_mhc == '\r') || (_mhc == '\n')) )
#define IsUnescaped(c) \
	( (_mhc=c), ((_mhc == '\"') || (_mhc == '\'') || (_mhc == '\\')) )
#define IsAlpha(c)    ( (_mhc=c), (IsUAlpha(c) || IsLAlpha(c)) )
#define IsUAlpha(c)   ( (_mhc=c), ((_mhc >= 'A') && (_mhc <= 'Z')) )
#define IsLAlpha(c)   ( (_mhc=c), ((_mhc >= 'a') && (_mhc <= 'z')) )
#define IsDigit(c)    ( (_mhc=c), ((_mhc >= '0') && (_mhc <= '9')) )
#define IsNegative(i) ( ((i) < 0) ? 1 : 0 )

#define uc(c) ( IsLAlpha(c) ? (char)((int)_mhc ^ 32) : _mhc )
#define lc(c) ( IsUAlpha(c) ? (char)((int)_mhc | 32) : _mhc )

#define max(a,b)  ( (a >= b) ? a : b )
#define min(a,b)  ( (a >= b) ? b : a )

#define Rnd(max)  ( 1 + (int)(max * rand() / (RAND_MAX + 1.0)) )

#define file_exists(f)    (0 == access(f, F_OK))
#define file_readable(f)  (0 == access(f, R_OK))
#define file_writable(f)  (0 == access(f, W_OK))

#define Log(fd, msg)  dprintf(fd, msg)

#define strMalloc(s)  Malloc( sizeof(char) * (1 + s) )
// Allocates memory for size+1 chars.
#define structDup(src, size)  memcpy(Malloc(size), src, size)

#define new(type)  ( (type*)Malloc(sizeof(type)) )

#define unless(cnd)  if(!(cnd))

#define IS_GCC_VERSION(major, minor)  (__GNUC__ > (major) || (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))

#define SKIP_WHITESPACE(cp)  while(       *cp==' ' || *cp=='\t' || *cp=='\r' || *cp=='\n') cp++
#define NEXT_WHITESPACE(cp)  while(*cp && *cp!=' ' && *cp!='\t' && *cp!='\r' && *cp!='\n') cp++

#endif // __MACROS_H

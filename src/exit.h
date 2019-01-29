#ifndef _EXIT_H
#define _EXIT_H

#include <stdlib.h>
#include <stdio.h>


#define EXIT_HELP 0
#define EXIT_SYNTAX 1
#define EXIT_NO_KEYWORDS EXIT_SYNTAX
#define EXIT_TOO_MANY_INPUTS 2
#define EXIT_TOO_MANY_KEYWORDS EXIT_TOO_MANY_INPUTS


/** Prints an error message on STDERR and exits.  */
__attribute__((format(printf, 2, 3)))
void die (unsigned int status, const char* error_format, ...);
#define die(status, ...)  do{ \
		fprintf(stderr, PROGNAME ": "); \
		fprintf(stderr, __VA_ARGS__); \
		exit(status); \
	}while(0)

/** Prints an error message on STDERR and abort()s.  */
__attribute__((format(printf, 1, 2)))
void fail (const char* error_format, ...);
#define fail(...)  do{ \
		fprintf(stderr, PROGNAME ": "); \
		fprintf(stderr, __VA_ARGS__); \
		abort(); \
	}while(0)


#endif  // _EXIT_H

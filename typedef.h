#ifndef __LIBTYPEDEF_H
#define __LIBTYPEDEF_H
#include <stdlib.h>


typedef  unsigned long int  ulint;
typedef    signed long int  slint;

#ifndef __USE_MISC /* see sys/types.h */
typedef  unsigned int  uint;
typedef  unsigned short  ushort;
#endif


typedef struct Word {
	char* str;
	uint  length;
} Word;

typedef struct Range {
	ulint start;
	ulint stop;
} Range;


#endif /* __LIBTYPEDEF_H */

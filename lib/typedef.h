#ifndef __LIBTYPEDEF_H
#define __LIBTYPEDEF_H

#include <stdlib.h>


typedef  unsigned long int  ulint;
typedef    signed long int  slint;

#ifndef __USE_MISC /* see sys/types.h */
typedef  unsigned int  uint;
typedef  unsigned short  ushort;
#endif

typedef  unsigned char  byte;


#endif /* __LIBTYPEDEF_H */

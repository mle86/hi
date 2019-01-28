#ifndef __MACROS_H
#define __MACROS_H
#include <string.h>


#define strequal(a, b)   (0 == strcmp((a), (b)))
#define striequal(a, b)  (0 == strcasecmp((a), (b)))

#define SKIP_WHITESPACE(cp)  while(         *(cp)==' ' || *(cp)=='\t' || *(cp)=='\r' || *(cp)=='\n')  (cp)++
#define NEXT_WHITESPACE(cp)  while(*(cp) && *(cp)!=' ' && *(cp)!='\t' && *(cp)!='\r' && *(cp)!='\n')  (cp)++


#endif /* __MACROS_H */

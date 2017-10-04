#ifndef __COMPILER_H
#define __COMPILER_H

#include "macros.h"


#if defined(__GNUC__)
	
	#if IS_GCC_VERSION(2,5)
		#define  __ATTR_noreturn              __attribute__(( noreturn ))
	#endif
	#if IS_GCC_VERSION(2,9)
		#define  __ATTR_format(fmt,idx,idx2)  __attribute__(( format(fmt,idx,idx2) ))
	#endif
	#if IS_GCC_VERSION(3,1)
		#define  __ATTR_always_inline         __attribute__(( always_inline ))
	#endif
	#if IS_GCC_VERSION(3,3)
		#define  __ATTR_nonnull(idx)          __attribute__(( nonnull(idx) ))
	#endif
	#if IS_GCC_VERSION(4,0)
		#define  __ATTR_sentinel              __attribute__(( sentinel ))
	#endif
	
#endif // GCC


#ifndef __ATTR_noreturn
	#define __ATTR_noreturn
#endif
#ifndef __ATTR_nonnull
	#define __ATTR_nonnull
#endif
#ifndef __ATTR_sentinel
	#define __ATTR_sentinel
#endif
#ifndef __ATTR_always_inline
	#define __ATTR_always_inline
#endif
#ifndef __ATTR_format
	#define __ATTR_format
#endif


#endif // __COMPILER_H

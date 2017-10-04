#ifndef __STR_C
#define __STR_C

#include "str.h"
#include "malloc.c"


unsigned short strEqual (char* a, char* b) {
	// Returns 1 if the strings a and b are equal,
	// returns 0 if they are not.
	if (a == b) return 1;
	if (! a)    return 0;
	if (! b)    return 0;
	do {
		if ( *a != *b )
			return 0; // not equal
	} while( *a++ && *b++ );
	return 1; // equal
}


unsigned short strnEqual (char* a, char* b, unsigned int n) {
	// Returns 1 if the strings a and b are equal,
	// returns 0 if they are not.
	if (a == b) return 1;
	if (! a)    return 0;
	if (! b)    return 0;
	while( n-- && (*a || *b) )
		if (*a++ != *b++)
			return 0;
	return 1; // equal
}


unsigned short strIEqual (char* a, char* b) {
	// Returns 1 if the strings a and b are equal,
	// returns 0 if they are not. Case is ignored.
	if (a == b) return 1;
	if (! a)    return 0;
	if (! b)    return 0;
	do {
		if ( lc(*a) != lc(*b) )
			return 0; // not equal
	} while( *a++ && *b++ );
	return 1; // equal
}


unsigned int strLen (char* s) {
	// Returns the character count of string s.
	// The terminating \0 character is not counted.
	register unsigned int c = 0;
	if (! s) return 0;
	while( *s++ ) c++;
	return c;
}


char* strxCat (char* a, char* b) {
	register char* dst, * dst2, c;
	unsigned int la, lb;
	if (! a) return b;
	if (! b) return a;
	if (! (la = strLen(a))) return b;
	if (! (lb = strLen(b))) return a;
	dst = dst2 = strMalloc(la + lb);
	while(( c = *a++ )) *dst2++ = c;
	while(( c = *b++ )) *dst2++ = c;
	*dst2 = '\0';
	return dst;
}

char* strCat (char* dest, char* src) {
	register char* d = dest;
	register char  c;
	if (! src) return dest;
	if (! dest) return src;
	while( *d ) d++;
	while(( c = *src++ )) *d++ = c;
	*d = '\0';
	return dest;
}

char* strDup (char* s) {
	// Returns a pointer to a duplicate of the given string s.
	register char* d;
	register unsigned int i = 0;
	if (! s) return NULL;
	d = Malloc( sizeof(char) * (1 + strLen(s)) );
	while(( d[i++] = *s++ ));;
	return d;
}


char* strTrim (char* s) {
	register unsigned int i = 0;
	register char c;
	char* n,
	    * nd,
	    * lastchar = NULL;
	short chk = 0;

	if (! s) return NULL;
	n = strMalloc(strLen(s));
	
	while(( c = *s++ )) {
		if (IsWhitespace(c)) {
			if (! chk) continue;
		} else {
			chk = 1;
			lastchar = &n[i];
		}
		n[i++] = c;
	}

	if (lastchar)
		*(lastchar + 1) = '\0';
	else
		n[i] = '\0';

	nd = strDup(n);
	free(n);
	return nd;
}


void strnCpy (unsigned int n, char* dst, char* src) {
	register unsigned int i;
	for( i = 0; i < n; i++ )
		dst[i] = src[i];
}



int striChr (char* s, char c, unsigned int offset) {
	// Returns the index of the first occurrence of char c in string s,
	// returns -1 if the char couldn't be found in s.
	register char t;
	unsigned int l = strLen(s);
	if (! l) return -1;
	if (offset >= l) return -1;
	while(( t = s[offset] )) {
		if (t == c)
			return offset;
		offset++;
	}
	return -1;
}


char* strChr (char* s, char c, unsigned int offset) {
	// Returns a pointer to the first occurrence of char c in string s,
	// return NULL if the char couldn't be found in s.
	if (! s) return NULL;
	s += offset;
	do {
		if (*s == c)
			return s;
	} while( *s++ );
	return NULL;
}
char* strrChr (char* s, char c, unsigned int offset) {
	// Returns a pointer to the last occurrence of char c in string s,
	// return NULL if the char couldn't be found in s.
	register unsigned int p = strLen(s);
	if (! p--) return NULL;
	p -= offset;
	do {
		if (s[p] == c)
			return &(s[p]);
	} while( s[p] && p-- );
	return NULL;
}

short strChrIn (char* s, char c) {
	register char t;
	while(( t = *s++ ))
		if (t == c)
			return 1;
	return 0;
}


char* strTail (char* s, char sep) {
	// Returns what string s contains after the last character sep.
	register char c, *l = NULL;
	while(( c = *s++ ))
		if (c == sep)
			l = s;
	return strDup(l);
}


char* strHead (char* s, char sep) {
	// Returns what string s contains before the first character sep.
	unsigned int p = striChr(s, sep, 0);
	register char* d = strMalloc(p);
	strnCpy(p, d, s);
	d[p] = '\0';
	return d;
}


unsigned int strChrCnt (char* s, char c) {
	// Counts all occurrences of character c in string s.
	register unsigned i = 0;
	register char t;
	while(( t = *s++ ))
		if (t == c)
			i++;
	return i;
}
unsigned int strChrgrpCnt (char* s, char gc) {
	// Counts all unique occurrences of character c in string s.
	register unsigned int i = 0;
	register char t;
	register short grp = 0;
	while(( t = *s++ )) {
		if (t == gc) {
			if (! grp) {
				grp++;
				i++;
			}
		} else grp = 0;
	}
	return i;
}


#define __chkChr(c,sep)                         \
	( (sep == WHITESPACE)                   \
		? ((c == ' ') || (c == '\t'))   \
		:  (c == sep)                   \
	)

char** strSplit (char* raw, char sep, unsigned int max, short multi, short trim) {
	// Splits a string into parts separated by sep.
	register char c;
	register unsigned int i=0, n=0, p=0;
	short escaped = 0;
	short chk = 0;
	char** r;
	char* tmp;
	uint l = strLen(raw);
	
	if (l < 1) return NULL;
	tmp = strMalloc(l);
	r = Malloc( sizeof(char*) * (2 + (max ? max : (multi ? strChrgrpCnt(raw, sep) : strChrCnt(raw, sep)))) );

	while(( c = raw[i++] )) {
		if (__chkChr(c, sep) && (! escaped) && ((! max) || (n < max))) {
			if (multi) {
				if (chk) continue;
				else chk++;
			}
			tmp[p] = '\0';
			r[n++] = trim ? strTrim(tmp) : strDup(tmp);
			p = 0;
		} else {
			chk = 0;
			if (escaped)
				escaped = 0;
			else if (c == '\\') {
				escaped++;
				continue; // Skip escaping backslash
			}
			tmp[p++] = c;
		}
	}
	if (p) {
		tmp[p] = '\0';
		r[n++] = trim ? strTrim(tmp) : strDup(tmp);
	}
	free(tmp);

	r[n] = NULL;
	return r;
}

char* strReplace (char* str, char hit, char* repl1, char* repl2) {
	char* r;
	char  c;
	register unsigned int i, occ = 0;
	unsigned int pos, posr, lenr1, lenr2, idx = 0, len = strLen(str);
	if ((! repl1) && (! repl2)) return str;
	for(i = 0; i < len; i++) if (str[i] == hit) occ++;
	if (! occ) return str;
	lenr1 = repl1 ? strLen(repl1) : 0;
	lenr2 = repl2 ? strLen(repl2) : 0;
	r = strMalloc(len + (occ * max(lenr1, lenr2)));
	for( pos=0,posr=0; pos < len; pos++ ) {
		c = str[pos];
		if (hit == c) {
			r[posr] = '\0';
			if (idx) {
				strCat(r, repl2);
				posr += lenr2;
				idx = 0;
				continue;
			} else {
				strCat(r, repl1);
				posr += lenr1;
				idx = 1;
				continue;
			}
		} else r[posr++] = c;
	}
	r[posr] = '\0';
	return r;
}


char* UC (char* s) {
	register int   l = strLen(s);
	register char* r = strMalloc(l);
	r[l] = '\0';
	for( l--; l >= 0; l-- )
		r[l] = uc(s[l]);
	return r;
}


char* LC (char* s) {
	register int   l = strLen(s);
	register char* r = strMalloc(l);
	r[l] = '\0';
	for( l--; l >= 0; l-- )
		r[l] = lc(s[l]);
	return r;
}

char* strStr (char* a, char* b) {
	// Returns the first occurence of string b in string a.
	// Returns NULL if string a does not contain string b.
	register unsigned int bp = 0;
	char* last = NULL;
	if (! (a && b && *a && *b))
		return NULL;
	if (a == b)
		return a;
	do {
		if (*a == b[bp]) {
			if (! *a)
				return last;
			if (! bp++)
				last = a;
		} else if (! b[bp])
			return last;
		else bp = 0;
	} while (*a++);
	return NULL;
}

#endif // __STR_C

#ifndef __STR_H
#define __STR_H

#include <stdlib.h>
#include "macros.h"
#include "malloc.h"

unsigned short strEqual (char* a, char* b);
// strEqual: Returns 1 if the given strings a and b
// are equal, 0 if they are not.

unsigned short strIEqual (char* a, char* b);
// strEqual: Returns 1 if the given strings a and b
// are equal, 0 if they are not. Case is ignored.

unsigned short strnEqual (char* a, char* b, unsigned int n);
// strEqual: Returns 1 if the first n chars of the
// given strings a and b are equal, 0 if they are not.

unsigned int strLen (char* s);
// strLen: Returns the length of the given string.
// The terminating \0 character is not counted.

char* strCat (char* dest, char* src);
// strCat: Appends string src to string dest.
// The trailing \0 char of dest is overwritten in the process.
// Make sure that dest is big enough!

char* strDup (char* s);
// strDup: Returns a pointer to a copy of the given string.

char* strTrim (char* s);
// strTrim: Removes whitespace character at the beginning and end of the string.

char** strSplit (char* raw, char sep, unsigned int max, short multi, short trim);
// strSplit: Separates a string into pieces.

void strnCpy (unsigned int n, char* dest, char* src);
// strnCpy: Copies n chars from src to dest.
// Caution: The trailing \0 char is not appended automatically if n is too low.

unsigned int strChrCnt (char* s, char c);
// strChrCnt: Counts all occurrences of character c in string s.

unsigned int strChrgrpCnt (char* s, char c);
// strChrgrpCnt: Counts all unique occurrences of character c in string s.

int striChr (char* s, char c, unsigned int offset);
// striChr: Returns the index of the first occurrence of char c in string s.

char* strChr (char* s, char c, unsigned int offset);
// strChr: Returns a pointer to the first occurrence of char c in string s.

short strChrIn (char* s, char c);
// strChrIn: Returns 1 if string s contains at least one char c.

char* strTail (char* s, char sep);
// strTail: Returns what string s contains after the last character sep.

char* strHead (char* s, char sep);
// strHead: Returns what string s contains before the first character sep.

char* UC (char* s);
// UC: Converts all chars in s to uppercase chars.

char* LC (char* s);
// LC: Converts all chars in s to lowercase chars.

char* strReplace (char* str, char hit, char* repl1, char* repl2);
// strReplace: Replaces all occurences of char hit in string str with repl1 and repl2.

char* strStr (char* a, char* b);
// Returns the first occurence of string b in string a.
// Returns NULL if string a does not contain string b.


#define WHITESPACE (-2)
#define strLine(s) strHead(s, '\n')

#endif // STR_H

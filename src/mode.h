#ifndef _MODE_H
#define _MODE_H


typedef enum mode {
	MODE_LINE,
	MODE_PARAGRAPH,
	MODE_WORDS,
	MODE_EXPLICIT,

	DEFAULT_MODE = MODE_LINE,
} Mode;


#endif  // _MODE_H

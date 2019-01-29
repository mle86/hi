#ifndef _COLOR_H
#define _COLOR_H

#include <stdbool.h>
#include <stdio.h>


// May be used externally:
#define COLOR_BOLD "[1m"       // printable ansi sequence for bold text.
#define COLOR_UNDERLINED "[4m" // printable ansi sequence for underlined text.
#define COLOR_SGR0 "[0m"       // printable ansi sequence for regular text.


// This type should be treated as if it were opaque.
typedef struct color {
	const char* ansi_code;
} Color;

extern const Color DEFAULT_COLOR;


/**
 * Tries to identify a color name.
 * If it's successful, it writes the Color to '*c' and returns true.
 * Else it returns false and does not touch 'c'.
 */
bool identify_color (const char* name, Color *c);

/**
 * Writes a colorized formatted message to the 'output' filehandle.
 */
__attribute__((format(printf, 3, 4)))
void colorizef (Color color, FILE* output, const char* format, ...);

/**
 * Writes a colorized message to the 'output' filehandle.
 */
void colorize (const Color color, FILE* output, const char* message);


#endif  // _COLOR_H

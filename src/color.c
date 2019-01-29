#include <strings.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include "color.h"

static const char ANSI_RED    [] = "[1;31m";
static const char ANSI_GREEN  [] = "[1;32m";
static const char ANSI_YELLOW [] = "[1;33m";
static const char ANSI_BLUE   [] = "[1;34m";
static const char ANSI_CYAN   [] = "[1;36m";
static const char ANSI_WHITE  [] = "[1;37m";
#define           ANSI_SGR0        COLOR_SGR0

const Color DEFAULT_COLOR = { .ansi_code = ANSI_YELLOW };


// Tries to identify a color name.
// If it's successful, it writes the Color to '*c' and returns true.
bool identify_color (const char* name, Color *c) {
	#define streq(a, b)  (0 == strcasecmp((a), (b)))
	#define emit(known_ansi_code)  do{ \
			c->ansi_code = known_ansi_code; \
			return true; \
		}while(0)

	if (streq(name, "gr") || streq(name, "gn") || streq(name, "green"))
		emit(ANSI_GREEN);
	if (streq(name, "re") || streq(name, "rd") || streq(name, "red"))
		emit(ANSI_RED);
	if (streq(name, "bl") || streq(name, "blue"))
		emit(ANSI_BLUE);
	if (streq(name, "ye") || streq(name, "yw") || streq(name, "yellow"))
		emit(ANSI_YELLOW);
	if (streq(name, "cy") || streq(name, "cn") || streq(name, "cyan"))
		emit(ANSI_CYAN);
	if (streq(name, "wh") || streq(name, "white"))
		emit(ANSI_WHITE);
	
	return false;
}

// Writes a colorized formatted message to the 'output' filehandle.
void colorizef (Color color, FILE* output, const char* format, ...) {
	if (!format || !format[0])
		return;

	fprintf(output, "%s", color.ansi_code);

	va_list va;
	va_start(va, format);
	vfprintf(output, format, va);
	va_end(va);

	fprintf(output, ANSI_SGR0);
}

// Writes a colorized message to the 'output' filehandle.
void colorize (Color color, FILE* output, const char* message) {
	if (!message || !message[0])
		return;

	fprintf(output, "%s%s" ANSI_SGR0, color.ansi_code, message);
}


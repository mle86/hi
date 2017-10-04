#include "hi.h"

char* Words [MAXWORDS];


int main (int argc, char** argv) {
	
	short Mark = DEFAULT_MARK;
	short Color = DEFAULT_COLOR;

	char* ParaBuf [MAXPARALINES];
	uint pline = 0;
	
	short found = 0;
	char buf [MAXLINELEN];
	char c;
	uint i, j, w=0;

	ssize_t s;
	
	while( (c = getopt(argc, argv, "wplc:hV")) != -1 )
	switch(c) {
		case 'w': Mark = MARK_WORD; break;
		case 'p': Mark = MARK_PARA; break;
		case 'l': Mark = MARK_LINE; break;
		case 'c': SetColor(optarg, &Color); break;
		case 'h': Help(); return 0;
		case 'V': Version(); return 0;
	}
	
	init();
	
	while( optind<argc && w<MAXWORDS ) {
		char* p = Prepare(argv[optind++], 0);
		for( i=0; i<w; i++ )
			if (strEqual(p, Words[i])) {
				found = 1;
				break;
			}
		if (! found)
			Words[w++] = p;
		else	free(p);
	}
	if (optind < argc) {
		fprintf(stderr, "%s: too many keywords (max. %i)\n", argv[0], MAXWORDS);
		return 1;
	}
	if (! w) {
		fprintf(stderr, "%s: no keywords\n", argv[0]);
		return 2;
	}

	found = 0;
	
	while(( s = read(0, buf, MAXLINELEN-1) ) > 0 ) {
		uint offs = 0; // offset from buf[0] for next line
		short wh = 1; // wh==1  means blank line so far
		buf[s] = '\0';
		for(i=0; i<=s; i++) {
			if (buf[i]=='\r' && (i+1)<=s && (buf[i+1]=='\n' || buf[i+1]=='\0')) {
				// Skip \r
				i++;
			} else if (buf[i]=='\n' || buf[i]=='\r' || buf[i]=='\0') {
				if (wh) {
					// NEW PARAGRAPH
					printf("\n");
					if (Mark==MARK_PARA) {
						PrintParagraph( ParaBuf, pline, found ? Color : 0, 1 );
						for(j=0; j<pline; j++) 
							free(ParaBuf[j]);
						pline = 0;
						found = 0;
					}
					if ((i+1)<=s && buf[i+1]=='\0')
						break;
				} else {
					// NEW LINE
					buf[i] = '\0';
					if (Mark==MARK_PARA) {
						ParaBuf[pline++] = strDup(&buf[offs]);
						if (! found)
							found = ScanLine( Words, w, &buf[offs], i-offs );
						if (pline >= MAXPARALINES) {
							// Line buffer full -- print it now.
							// May cause slight highlighting glitch.
							PrintParagraph( ParaBuf, pline, found ? Color : 0, 1 );
							for(j=0; j<pline; j++)
								free(ParaBuf[j]);
							pline = 0;
						}
					} else if (Mark==MARK_LINE) {
						char* ptr = &buf[offs];
						PrintParagraph( &ptr, 1, ScanLine( Words, w, ptr, i-offs ) ? Color : 0, 1 );
					}
					wh = 1;
					
					if ((i+1)<=s && buf[i+1]=='\0')
						break;
				}
				offs = i + 1;
			} else if (buf[i]!=' ' && buf[i]!='\t')
				wh = 0;
		}
	}
	
	if (Mark==MARK_PARA && pline)
		PrintParagraph( ParaBuf, pline, found ? Color : 0, 1 );
	
	switch(s) {
	  case -1:
		fprintf(stderr, "%s: read error\n", argv[0]);
		return 3;
	  default:
		return 0;
	}
}


void PrintParagraph (char** Lines, uint linecnt, short color, short brk) {
	register uint i;
	if (color)
		printf("[1;%im", color);
	for(i=0; i<linecnt; i++)
		printf( brk ? "%s\n" : "%s", Lines[i] );
	if (color) {
		printf("[0m");
		fflush(NULL);
	}
}


short ScanLine (char** Words, uint wordcnt, char* rawline, uint len) {
	register uint i;
	char* line;
	if (! len)
		return 0;
	line = strMalloc(len);
	for(i=0; i<=len; i++)
		line[i] = lc(rawline[i]);
	for(i=0; i<wordcnt; i++)
		if (strStr(line, Words[i]))
			return 1;
	return 0;
}


char* Prepare (char* raw, uint len) {
	char* ret;
	register uint p;
	if (! len)
		len = strLen(raw);
	ret = strMalloc(len);
	for( p=0; p<len && raw[p]; p++ )
		ret[p] = lc(raw[p]);
	ret[p] = '\0';
	return ret;
}


void init (void) {
	register uint i;
	for( i=0; i<MAXWORDS; i++ )
		Words[i] = NULL;
}


void SetColor (char* arg, short* color) {
	if (strIEqual(arg, "gr") || strEqual(arg, "gn") || strIEqual(arg, "green"))
		*color = ANSI_GREEN;
	else if (strIEqual(arg, "re") || strIEqual(arg, "rd") || strIEqual(arg, "red"))
		*color = ANSI_RED;
	else if (strIEqual(arg, "bl") || strIEqual(arg, "blue"))
		*color = ANSI_BLUE;
	else if (strIEqual(arg, "ye") || strIEqual(arg, "yw") || strIEqual(arg, "yellow"))
		*color = ANSI_YELLOW;
	else if (strIEqual(arg, "cy") || strIEqual(arg, "cn") || strIEqual(arg, "cyan"))
		*color = ANSI_CYAN;
	else if (strIEqual(arg, "wh") || strIEqual(arg, "white"))
		*color = ANSI_WHITE;
}


void Help (void) { printf(
	M1 PROGNAME M0 "  reads text  from  standard input  and  highlights\n"
	"those paragraphs/lines containing at least one of the given keywords.\n"
	"Usage: " M1 PROGNAME M0 " [" M1 "OPTIONS" M0 "] " M1 "KEYWORD" M0 " [" M1 "..." M0 "]\n"
	"Options:\n"
	M1 "  -p        " M0 "highlight paragraphs containing at least one of the " M1 "KEYWORD" M0 "S.\n"
	M1 "  -l        " M0 "highlight lines containing at least one of the " M1 "KEYWORD" M0 "S.\n"
//	M1 "  -w        " M0 "highlight only KEYWORD" M0 "S\n"
	M1 "  -c COLOR  " M0 "select highlighting color, choose from\n"
	   "            "    "white, red, green, blue, yellow, cyan.\n"
	M1 "  -h        " M0 "this help.\n"
	M1 "  -V        " M0 "program version information.\n"
	"Options " M1 "-plw" M0 " are mutually exclusive.\n"
	"Defaults are " M1 "-p -c yellow" M0 ".\n"
	"\n"
); exit(0); }


void Version (void) { printf(
	PROGNAME " v" VERSION "\n"
	"Written by Maximilian Eul <mle@multinion.de>, February 2007.\n"
	"\n"
); exit(0); }

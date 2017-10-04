#include "hi.h"

char* Words [MAXWORDS];
char buf [MAXLINELEN];
ulint Readlen;


int main (int argc, char** argv) {
	
	short Mark = DEFAULT_MARK;
	short Color = DEFAULT_COLOR;

	char* ExpLines = NULL;
	char* ParaBuf [MAXPARALINES];
	uint pline = 0;
	
	bool found = false;
	char c;
	ulint i;
	uint j, w=0;

	ssize_t s;

	Readlen = min((ulint)SSIZE_MAX, MAXLINELEN) - 1;
	
	while( (c = getopt(argc, argv, "wplL:c:hV")) != -1 )
	switch(c) {
		case 'w': Mark = MARK_WORD; break;
		case 'p': Mark = MARK_PARA; break;
		case 'l': Mark = MARK_LINE; break;
		case 'L': Mark = MARK_EXPLICIT; ExpLines = strDup(optarg); break;
		case 'c': SetColor(optarg, &Color); break;
		case 'h': Help(); return RET_HELP;
		case 'V': Version(); return RET_HELP;
	}
	
	init();

	if (Mark == MARK_EXPLICIT)
		return ExplicitOnly(argv[0], ExpLines, Color);
	
	while( optind<argc && w<MAXWORDS ) {
		char* p = Prepare(argv[optind++], 0);
		for( i=0; i<w; i++ )
			if (strEqual(p, Words[i])) {
				found = true;
				break;
			}
		if (! found)
			Words[w++] = p;
		else	free(p);
	}
	if (optind < argc) {
		fprintf(stderr, "%s: too many keywords (max. %i)\n", argv[0], MAXWORDS);
		return RET_TOOMANYKEYWORDS;
	}

	found = false;
	for(i=0; i<w; i++)
		if (Words[i][0]) {
			found = true;
			break;
		}
	if (! found) {
		fprintf(stderr, "%s: no keywords\n", argv[0]);
		return RET_NOKEYWORDS;
	}

	found = false;
	
	while(( s = read(0, buf, Readlen) ) > 0 ) {
		ulint offs = 0; // offset from buf[0] for next line
		bool wh = true; // wh==true  means blank line so far
		buf[s] = '\0';
		for(i=0; i<=s; i++) {
			// current line starts at &buf[offs]
			
			if (buf[i]=='\r' && (i+1)<=s && (buf[i+1]=='\n' || buf[i+1]=='\0')) {
				// Skip \r
				i++;
			} else if (buf[i]=='\n' || buf[i]=='\r' || buf[i]=='\0') {
				if (wh) {
					// NEW PARAGRAPH
					printf("\n");
					if (Mark==MARK_PARA) {
						PrintParagraph( ParaBuf, pline, found ? Color : 0, true );
						for(j=0; j<pline; j++) 
							free(ParaBuf[j]);
						pline = 0;
						found = false;
					}
					if ((i+1)<=s && buf[i+1]=='\0')
						break;
				} else {
					// NEW LINE
					buf[i] = '\0';
					// last line: buf[ offs .. i ]
					
					if (Mark==MARK_PARA) {
						ParaBuf[pline++] = strDup(&buf[offs]);
						if (! found)
							found = ScanLine( Words, w, &buf[offs], i-offs, NULL, NULL );
						if (pline >= MAXPARALINES) {
							// Line buffer full -- print it now.
							// May cause slight highlighting glitch.
							PrintParagraph( ParaBuf, pline, found ? Color : 0, true );
							for(j=0; j<pline; j++)
								free(ParaBuf[j]);
							pline = 0;
						}
					} else if (Mark==MARK_LINE) {
						char* lptr = &buf[offs];
						PrintParagraph( &lptr, 1, ScanLine( Words, w, lptr, i-offs, NULL, NULL)
						                           ? Color : 0,
						                true );
					} else if (Mark==MARK_WORD) {
						char* lptr = &buf[offs];
						char* pos;
						ulint len;
						while(( ScanLine(Words, w, lptr, i-offs, &pos, &len) )) {
							register char saved1 = *pos;
							register char saved2 = *(pos + len);
							*(pos + len) = *pos = '\0'; // cut before and after keyword

							//printf("%s[1;%im%c%s[0m", lptr, Color, saved1, pos+1);
							PrintParagraph(&lptr, 1, 0, false);
							*pos = saved1;
							PrintParagraph(&pos, 1, Color, false);

							lptr = pos + len; // skip word,
							*lptr = saved2; // uncut
						}
						printf("%s\n", lptr);
					}
					
					wh = true;
					if ((i+1)<=s && buf[i+1]=='\0')
						break;
				}
				offs = i + 1;
			} else if (buf[i]!=' ' && buf[i]!='\t')
				wh = false;
		}
	}
	
	if (Mark==MARK_PARA && pline)
		PrintParagraph( ParaBuf, pline, found ? Color : 0, true );
	
	switch(s) {
	  case -1:
		fprintf(stderr, "%s: read error\n", argv[0]);
		return RET_READERROR;
	  default:
		return RET_OK;
	}
}


int ExplicitOnly (char* argv0, char* ExpLines, int Color) {
	uint lstart = 0,
	     lstop  = 0,
	     curline = 1;
	char* p = ExpLines;
	ssize_t s;

	if (! ExpLines && ! IsDigit(ExpLines[0])) {
		fprintf(stderr, "%s: no lines given\n", argv0);
		return RET_NOLINES;
	}

	while( IsDigit(*p) )  p++;
	lstart = atoi(ExpLines);
	lstop = (*(p++)=='-' && IsDigit(*p)) ? atoi(p) : lstart;

	while(( s = read(0, buf, Readlen) ) > 0 ) {
		register unsigned int i;
		uint offs = 0; // offset from buf[0] for next line
		for(i=0; i<=s; i++)
			if (buf[i]=='\r' && (i+1)<=s && (buf[i+1]=='\n' || buf[i+1]=='\0')) {
				// Skip \r
				i++;
			} else if (buf[i]=='\n' || buf[i]=='\r' || buf[i]=='\0') {
				// New line
				char* ptr = &buf[offs];
				buf[i] = '\0';
				PrintParagraph(&ptr, 1, (curline >= lstart && curline <= lstop) ? Color : 0, true);
				curline++;
				offs = i + 1;

				if ((i+1)<=s && buf[i+1]=='\0')
					break;
			}
	}

	switch(s) {
	  case -1:
		fprintf(stderr, "%s: read error\n", argv0);
		return RET_READERROR;
	  default:
		return RET_OK;
	}
}


void PrintParagraph (char** Lines, uint linecnt, short color, bool brk) {
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


bool ScanLine (char** Words, uint wordcnt, char* rawline, ulint len,  char* *savePos, ulint *saveWlen) {
	register ulint i;
	char* line;
	if (! len)
		return false;
	line = strMalloc(len);
	for(i=0; i<=len; i++)
		line[i] = lc(rawline[i]);
	for(i=0; i<wordcnt; i++) {
		register char* pos;
		if ((pos = strStr(line, Words[i]))) {
			if (savePos)  *savePos = rawline + (pos - line);
			if (saveWlen)  *saveWlen = strLen(Words[i]);
			free(line);
			return true;
		}
	}
	free(line);
	return false;
}


char* Prepare (char* raw, ulint len) {
	char* ret;
	register ulint p;
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
	M1 "  -w        " M0 "highlight only KEYWORD" M0 "S\n"
	M1 "  -L nn     " M0 "highlight only the given line(s), no " M1 "KEYWORD" M0 "S needed.\n"
	   "            " M1 "nn: " M0 "either a single line number (e.g. " M1 "71" M0 ") or a range (e.g. " M1 "1-9" M0 ").\n"
	M1 "  -c COLOR  " M0 "select highlighting color, choose from\n"
	   "            "    "white, red, green, blue, yellow, cyan.\n"
	M1 "  -h        " M0 "this help.\n"
	M1 "  -V        " M0 "program version information.\n"
	"Options " M1 "-plwL" M0 " are mutually exclusive.\n"
	"Defaults are " M1 "-p -c yellow" M0 ".\n"
	"\n"
); exit(0); }


void Version (void) { printf(
	PROGNAME " v" VERSION "\n"
	"Written by Maximilian Eul <mle@multinion.de>, September 2008.\n"
	"\n"
); exit(0); }

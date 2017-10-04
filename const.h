#ifndef __CONST_H
#define __CONST_H


#define VERSION  "1.2"

#define DEFAULT_MARK   MARK_PARA
#define DEFAULT_COLOR  ANSI_YELLOW


#define MAXWORDS 20
#define MAXLINELEN 8192
#define MAXPARALINES 4096

enum {
	MARK_WORD,
	MARK_PARA,
	MARK_LINE,
	MARK_EXPLICIT
};

#define ANSI_RED     31
#define ANSI_GREEN   32
#define ANSI_YELLOW  33
#define ANSI_BLUE    34
#define ANSI_WHITE   37
#define ANSI_CYAN    36

#define M1 "[1m"
#define M0 "[0m"


#define RET_OK		0
#define RET_HELP	0
#define RET_TOOMANYKEYWORDS	1
#define RET_NOKEYWORDS	2
#define RET_READERROR	3
#define RET_NOLINES	4


#endif // __CONST_H

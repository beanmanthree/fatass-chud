#ifndef ANSI_H
#define ANSI_H

#include <stdio.h>

#define ANSI_CSI "\x1b["

#define ANSI_RESET "\x1b[0m"
#define ANSI_BOLD "\x1b[1m"
#define ANSI_DIM "\x1b[2m"
#define ANSI_ITALIC "\x1b[3m"
#define ANSI_UNDERLINE "\x1b[4m"
#define ANSI_SLOW_BLINK "\x1b[5m"
#define ANSI_FAST_BLINK "\x1b[6m"
#define ANSI_INVERSE "\x1b[7m"
#define ANSI_HIDDEN "\x1b[8m"
#define ANSI_STRIKE "\x1b[9m"

#define ANSI_FG_DEFAULT "\x1b[39m"
#define ANSI_FG_BLACK "\x1b[30m"
#define ANSI_FG_RED "\x1b[31m"
#define ANSI_FG_GREEN "\x1b[32m"
#define ANSI_FG_YELLOW "\x1b[33m"
#define ANSI_FG_BLUE "\x1b[34m"
#define ANSI_FG_MAGENTA "\x1b[35m"
#define ANSI_FG_CYAN "\x1b[36m"
#define ANSI_FG_WHITE "\x1b[37m"

#define ANSI_FG_BRIGHT_BLACK "\x1b[90m"
#define ANSI_FG_BRIGHT_RED "\x1b[91m"
#define ANSI_FG_BRIGHT_GREEN "\x1b[92m"
#define ANSI_FG_BRIGHT_YELLOW "\x1b[93m"
#define ANSI_FG_BRIGHT_BLUE "\x1b[94m"
#define ANSI_FG_BRIGHT_MAGENTA "\x1b[95m"
#define ANSI_FG_BRIGHT_CYAN "\x1b[96m"
#define ANSI_FG_BRIGHT_WHITE "\x1b[97m"

#define ANSI_BG_DEFAULT "\x1b[49m"
#define ANSI_BG_BLACK "\x1b[40m"
#define ANSI_BG_RED "\x1b[41m"
#define ANSI_BG_GREEN "\x1b[42m"
#define ANSI_BG_YELLOW "\x1b[43m"
#define ANSI_BG_BLUE "\x1b[44m"
#define ANSI_BG_MAGENTA "\x1b[45m"
#define ANSI_BG_CYAN "\x1b[46m"
#define ANSI_BG_WHITE "\x1b[47m"

#define ANSI_BG_BRIGHT_BLACK "\x1b[100m"
#define ANSI_BG_BRIGHT_RED "\x1b[101m"
#define ANSI_BG_BRIGHT_GREEN "\x1b[102m"
#define ANSI_BG_BRIGHT_YELLOW "\x1b[103m"
#define ANSI_BG_BRIGHT_BLUE "\x1b[104m"
#define ANSI_BG_BRIGHT_MAGENTA "\x1b[105m"
#define ANSI_BG_BRIGHT_CYAN "\x1b[106m"
#define ANSI_BG_BRIGHT_WHITE "\x1b[107m"

char ANSI_getch(void);

void ANSI_fg256(int idx);
void ANSI_bg256(int idx);

void ANSI_fgRgb(int r, int g, int b);
void ANSI_bgRgb(int r, int g, int b);

void ANSI_moveUp(int n);
void ANSI_moveDown(int n);
void ANSI_moveForward(int n);
void ANSI_moveBackward(int n);

void ANSI_moveTo(int r, int c);

void ANSI_clearScreen(void);

void ANSI_hideCursor(void);
void ANSI_showCursor(void);

void ANSI_saveCursor(void);
void ANSI_restoreCursor(void);

#endif

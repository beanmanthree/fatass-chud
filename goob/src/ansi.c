#include "ansi.h"

#include <stdio.h>

void ANSI_fg256(int idx) {
    printf(ANSI_CSI "38;5;%dm", idx);
}

void ANSI_bg256(int idx) {
    printf(ANSI_CSI "48;5;%dm", idx);
}

void ANSI_fgRgb(int r, int g, int b) {
    printf(ANSI_CSI "38;2;%d;%d;%dm", r, g, b);
}

void ANSI_bgRgb(int r, int g, int b) {
    printf(ANSI_CSI "48;2;%d;%d;%dm", r, g, b);
}

void ANSI_moveUp(int n) {
    printf(ANSI_CSI "%dA", n);
}

void ANSI_moveDown(int n) {
    printf(ANSI_CSI "%dB", n);
}

void ANSI_moveForward(int n) {
    printf(ANSI_CSI "%dC", n);
}

void ANSI_moveBackward(int n) {
    printf(ANSI_CSI "%dD", n);
}

void ANSI_moveTo(int r, int c) {
    printf(ANSI_CSI "%d;%dH", r, c);
}

void ANSI_clearScreen(void) {
    printf(ANSI_CSI "2J");
}

void ANSI_hideCursor(void) {
    printf(ANSI_CSI "?25l");
}

void ANSI_showCursor(void) {
    printf(ANSI_CSI "?25h");
}

void ANSI_saveCursor(void) {
    printf(ANSI_CSI "s");
}

void ANSI_restoreCursor(void) {
    printf(ANSI_CSI "u");
}

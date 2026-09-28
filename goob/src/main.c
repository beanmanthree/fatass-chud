#include <stdio.h>

#include "ansi.h"

void getTerminalDimensions(int* x0Out, int* y0Out, int* x1Out, int* y1Out) {
    int x0 = 1, y0 = 1;
    for (char c = ' '; c != '\n'; c = ANSI_getch()) {
        switch (c) {
            case 'w':
                if (y0 > 1) --y0;
                break;
            case 's':
                ++y0;
                break;
            case 'a':
                if (x0 > 1) --x0;
                break;
            case 'd':
                ++x0;
                break;
        }
        ANSI_clearScreen();
        ANSI_moveTo(y0, x0);
        printf(ANSI_FG_BRIGHT_RED ANSI_INVERSE);
        printf("Use WASD to move this to the");
        ANSI_moveTo(y0 + 1, x0);
        printf("top left of your terminal");
        printf(ANSI_RESET);
        fflush(stdout);
    }
    int x1 = x0, y1 = y0;
    for (char c = ' '; c != '\n'; c = ANSI_getch()) {
        switch (c) {
            case 'w':
                if (y1 > y0) --y1;
                break;
            case 's':
                ++y1;
                break;
            case 'a':
                if (x1 > x0) --x1;
                break;
            case 'd':
                ++x1;
                break;
        }
        ANSI_clearScreen();
        for (int i = x0; i <= x1; ++i) {
            ANSI_moveTo(y0, i);
            putchar('#');
            ANSI_moveTo(y1, i);
            putchar('#');
        }
        for (int i = y0; i <= y1; ++i) {
            ANSI_moveTo(i, x0);
            putchar('#');
            ANSI_moveTo(i, x1);
            putchar('#');
        }
        fflush(stdout);
    }
    *x0Out = x0;
    *y0Out = y0;
    *x1Out = x1;
    *y1Out = y1;
}

int main() {
    ANSI_hideCursor();
    int x0, y0, x1, y1;
    getTerminalDimensions(&x0, &y0, &x1, &y1);
    ANSI_showCursor();
    return 0;
}

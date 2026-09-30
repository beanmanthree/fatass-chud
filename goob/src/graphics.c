#include "graphics.h"

#include <math.h>
#include <stdio.h>

#include "ansi.h"
#include "utils.h"

void GRAPHICS_getTerminalDimensions(int* x0Out, int* y0Out, int* x1Out, int* y1Out) {
    const char tl = '/', tr = '\\', bl = '\\', br = '/';
    const char h = '-', v = '|';
    int x0 = 1, y0 = 1;
    printf(ANSI_BOLD);
    for (char c = ' '; c != '\n'; c = getch()) {
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
        putchar(tl);
        fflush(stdout);
    }
    int x1 = x0 + 1, y1 = y0 + 1;
    for (char c = ' '; c != '\n'; c = getch()) {
        switch (c) {
            case 'w':
                if (y1 > y0 + 1) --y1;
                break;
            case 's':
                ++y1;
                break;
            case 'a':
                if (x1 > x0 + 1) --x1;
                break;
            case 'd':
                ++x1;
                break;
        }
        ANSI_clearScreen();
        ANSI_moveTo(y0, x0);
        putchar(tl);
        for (int i = x0 + 1; i < x1; ++i) putchar(h);
        putchar(tr);
        ANSI_moveTo(y1, x0);
        putchar(bl);
        for (int i = x0 + 1; i < x1; ++i) putchar(h);
        putchar(br);
        for (int i = y0 + 1; i < y1; ++i) {
            ANSI_moveTo(i, x0);
            putchar(v);
            ANSI_moveTo(i, x1);
            putchar(v);
        }
        fflush(stdout);
    }
    printf(ANSI_RESET);
    *x0Out = x0;
    *y0Out = y0;
    *x1Out = x1;
    *y1Out = y1;
}

void GRAPHICS_put8(int value) {
    const char lkp[8] = {'.', '-', ':', '=', '+', '*', '#', '@'};
    value = value < -8 ? -8 : value > 8 ? 8 : value;
    if (value < 0) {
        printf(ANSI_DIM);
        putchar(lkp[-value - 1]);
        printf(ANSI_RESET);
    } else if (value > 0) putchar(lkp[value - 1]);
    else putchar(' ');
}

void GRAPHICS_putf(float value) {
    GRAPHICS_put8(roundf(value * 8));
}

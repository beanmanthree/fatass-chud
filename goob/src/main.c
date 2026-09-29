#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ansi.h"
#include "graphics.h"

int main(int argc, char *argv[]) {
    ANSI_hideCursor();
    int x0, y0, x1, y1;
    if (argc == 3) {
        x0 = 0;
        y0 = 0;
        x1 = atoi(argv[1]);
        y1 = atoi(argv[2]);
    } else {
        GRAPHICS_getTerminalDimensions(&x0, &y0, &x1, &y1);
    }
    for (char c = ' '; c != '\n'; c = ANSI_getch()) {
        ANSI_clearScreen();
        for (int i = y0; i <= y1; ++i) {
            ANSI_moveTo(i, x0);
            for (int j = x0; j <= x1; ++j) {
                float d = 10;
                float x = i / d, y = j / d;
                GRAPHICS_drawf(sinf(x + y) + cosf(y - x) - sin(x * y));
            }
        }
        switch (c) {
            case 'w':
                --y0;
                --y1;
                break;
            case 's':
                ++y0;
                ++y1;
                break;
            case 'a':
                --x0;
                --x1;
                break;
            case 'd':
                ++x0;
                ++x1;
                break;
        }
    }
    ANSI_showCursor();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ansi.h"
#include "graphics.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    void sleep(unsigned int ms) {
        Sleep(ms);
    }
#else
    #include <time.h>
    #include <errno.h>

    void sleep(unsigned int ms) {
        struct timespec ts;
        ts.tv_sec = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000000L;
        while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
    }
#endif

float getBorderInverse(int x0, int y0, int x1, int y1, float a, float b, float falloff) {
    float mn0 = a - x0 < x1 - a ? a - x0: x1 - a;
    float mn1 = b - y0 < y1 - b ? b - y0 : y1 - b;
    return 1.f / ((mn0 < mn1 ? mn0 : mn1) * falloff + 1);
}

float getMetaBall(float x, float y, float radius, float a, float b) {
    return radius / hypotf(x - a, y - b);
}

void calculateParametric(int x0, int y0, int x1, int y1, float t, float a, float b, float c, float d, float* outx, float* outy) {
    *outx = ((x1 - x0) * sinf(a * t + b) + (x0 + x1)) / 2;
    *outy = ((y1 - y0) * sinf(c * t + d) + (y0 + y1)) / 2;
}

float getContour(int x0, int y0, int x1, int y1, float a, float b, float t, float* balls, int ballCount) {
    bool big = false, small = false;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            float sum = getBorderInverse(x0, y0, x1, y1, a + i, b + j, 8);
            for (int k = 0; k < ballCount; ++k) {
                float x, y;
                calculateParametric(x0, y0, x1, y1, t, balls[k * 5 + 1], balls[k * 5 + 2], balls[k * 5 + 3], balls[k * 5 + 4], &x, &y);
                sum += getMetaBall(x, y, balls[k * 5 + 0], a + i, b + j);
            }
            if (sum > 1) big = true;
            else small = true;
        }
    }
    return big && small ? 1.f : 0.f;
}

int main(int argc, char *argv[]) {
    ANSI_hideCursor();
    int x0, y0, x1, y1;
    if (argc == 3) {
        x0 = 0;
        y0 = 0;
        x1 = atoi(argv[1]);
        y1 = atoi(argv[2]); 
    } else if (argc == 5) {
        x0 = atoi(argv[1]);
        y0 = atoi(argv[2]);
        x1 = atoi(argv[3]);
        y1 = atoi(argv[4]);
    } else {
        GRAPHICS_getTerminalDimensions(&x0, &y0, &x1, &y1);
    }
    ++x1, ++y1;
    const int ballCount = 20;
    float* balls = (float*)malloc(ballCount * 5 * sizeof(float));
    for (int i = 0; i < ballCount; ++i) {
        balls[i * 5 + 0] = rand() / (float)RAND_MAX * 1 + 1;
        balls[i * 5 + 1] = rand() / (float)RAND_MAX * 4;
        balls[i * 5 + 2] = rand() / (float)RAND_MAX * 4;
        balls[i * 5 + 3] = rand() / (float)RAND_MAX * 4;
        balls[i * 5 + 4] = rand() / (float)RAND_MAX * 4;
    }
    for (float t = 0; t < 1000; t += 0.005) {
        ANSI_clearScreen();
        for (int j = y0; j < y1; ++j) {
            ANSI_moveTo(j, x0);
            for (int i = x0; i < x1; ++i) {
                GRAPHICS_putf(getContour(x0, y0, x1, y1,(float)i, (float)j, t, balls, ballCount));
            }
        }
        fflush(stdout);
        sleep(32);
    }
    ANSI_showCursor();
    return 0;
}

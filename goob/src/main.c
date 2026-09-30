#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ansi.h"
#include "graphics.h"
#include "utils.h"

float getBorderInverse(float x0, float y0, float x1, float y1, float a, float b, float falloff) {
    float mn0 = a - x0 < x1 - a ? a - x0: x1 - a;
    float mn1 = b - y0 < y1 - b ? b - y0 : y1 - b;
    return 1.0001f / ((mn0 < mn1 ? mn0 : mn1) * falloff + 1.f);
}

float getMetaBall(float x, float y, float radius, float a, float b) {
    return radius / hypotf(x - a, y - b);
}

float* generateBalls(int ballCount) {
    float* balls = (float*)malloc(ballCount * 5 * sizeof(float));
    for (int i = 0; i < ballCount; ++i) {
        balls[i * 5 + 0] = 1.f;
        balls[i * 5 + 1] = rand() / (float)RAND_MAX * 8 - 4;
        balls[i * 5 + 2] = rand() / (float)RAND_MAX * 8 - 4;
        balls[i * 5 + 3] = rand() / (float)RAND_MAX * 8 - 4;
        balls[i * 5 + 4] = rand() / (float)RAND_MAX * 8 - 4;
    }
    return balls;
}

void calculateParametric(float x0, float y0, float x1, float y1, float t, float a, float b, float c, float d, float* outx, float* outy) {
    *outx = ((x1 - x0) * cosf(a * t + b) + (x0 + x1)) / 2;
    *outy = ((y1 - y0) * sinf(c * t + d) + (y0 + y1)) / 2;
}

float calcPoint(int w, int h, float a, float b, float t, float* balls, int ballCount) {
    float sum = getBorderInverse(0, 0, w, h, a, b, 8);
    for (int k = 0; k < ballCount; ++k) {
        float x, y;
        calculateParametric(0, 0, w, h, t, balls[k * 5 + 1], balls[k * 5 + 2], balls[k * 5 + 3], balls[k * 5 + 4], &x, &y);
        sum += getMetaBall(x, y, balls[k * 5 + 0], a, b);
    }
    return sum;
}

void fillMatrix(bool* mat, int w, int h, float t, float* balls, int ballCount) {
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            mat[i * h + j] = calcPoint(w - 1, h - 1, i, j, t, balls, ballCount) > 1;
        }
    }
}

void getRGB(int x0, int y0, int x1, int y1, int t, float mul, int i, int j, int* outr, int* outg, int* outb) {
    t *= mul;
    float x = (float)(i - x0) / (float)(x1 - x0);
    float y = (float)(j - y0) / (float)(y1 - y0);
    double waver = sin(x * 5.0 + t * 1.5) * 0.5 + sin(y * 3.0 - t * 1.0) * 0.5;
    double waveg = sin(y * 4.0 + t * 2.0) * 0.5 + sin(x * 2.0 + t * 0.5) * 0.5;
    double waveb = sin((x + y) * 3.5 - t * 1.2) * 0.5 + sin(x * 4.0 + t * 2.5) * 0.5;
    double normr = (waver * 0.5) + 0.5;
    double normg = (waveg * 0.5) + 0.5;
    double normb = (waveb * 0.5) + 0.5;
    *outr = (int)(normr * 255.0);
    *outg = (int)(normg * 255.0);
    *outb = (int)(normb * 255.0);
}

void drawContour(char* lkp, int x0, int y0, int x1, int y1, bool* mat, int w, int h, float t, float* balls, int ballCount) {
    fillMatrix(mat, w, h, t, balls, ballCount);
    bool last = false;
    for (int j = y0; j <= y1; ++j) {
        for (int i = x0; i <= x1; ++i) {
            int a = i - x0, b = j - y0;
            bool big = false, small = false;
            for (int ai = 0; ai < 2; ++ai) {
                for (int bi = 0; bi < 2; ++bi) {
                    if (mat[(a + ai) * h + (b + bi)]) big = true;
                    else small = true;
                }
            }
            if (big && small) {
                if (!last) ANSI_moveTo(j, i);
                last = true;
                int r, g, b;
                getRGB(x0, y0, x1, y1, t, 100, i, j, &r, &g, &b);
                ANSI_fgRgb(r, g, b);
                putchar(lkp[a * (h - 1) + b]);
            } else last = false;
        }
        last = false;
    }
}

int main(int argc, char *argv[]) {
    ANSI_hideCursor();
    int x0, y0, x1, y1;
    if (argc == 3) {
        x0 = 1;
        y0 = 1;
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
    const int w = x1 - x0 + 2, h = y1 - y0 + 2;
    bool* mat = malloc(w * h * sizeof(bool));
    char* lkp = malloc((w - 1) * (h - 1) * sizeof(char));
    const char chars[8] = {'@', '#', '!', '$', '%', '&', '8', '?'};
    for (int i = (w - 1) * (h - 1) - 1; i >= 0; --i) lkp[i] = chars[rand() % 8];
    const int ballCount = 0;
    float* balls = generateBalls(ballCount);
    char key;
    for (float t = 0; getchNB(&key) == 0 || key != '\n'; t += 0.001) {
        ANSI_clearScreen();
        drawContour(lkp, x0, y0, x1, y1, mat, w, h, t, balls, ballCount);
        fflush(stdout);
        sleepms(16);
    }
    ANSI_showCursor();
    return 0;
}

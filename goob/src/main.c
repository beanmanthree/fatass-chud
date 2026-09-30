#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ansi.h"
#include "graphics.h"
#include "utils.h"

float getBorderInverse(float x0, float y0, float x1, float y1, float a, float b, float falloff) {
    float mn0 = a - x0 < x1 - a ? a - x0: x1 - a;
    float mn1 = b - y0 < y1 - b ? b - y0 : y1 - b;
    return 1.00001f / ((mn0 < mn1 ? mn0 : mn1) * falloff + 1.f);
}

float getMetaBall(float x, float y, float radius, float a, float b) {
    return radius / hypotf(x - a, y - b);
}

float* generateBalls(int ballCount) {
    float* balls = (float*)malloc(ballCount * 5 * sizeof(float));
    for (int i = 0; i < ballCount; ++i) {
        balls[i * 5 + 0] = rand() / (float)RAND_MAX + 1.f;
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

float calcPoint(int w, int h, float a, float b, float* balls, float* centers, int ballCount) {
    float sum = getBorderInverse(0, 0, w, h, a, b, 8);
    for (int k = 0; k < ballCount; ++k) {
        sum += getMetaBall(centers[k * 2], centers[k * 2 + 1], balls[k * 5 + 0], a, b);
    }
    return sum;
}

void fillMatrix(bool* mat, int w, int h, float t, float* balls, float* centers, int ballCount) {
    for (int k = 0; k < ballCount; ++k) {
        calculateParametric(0, 0, w, h, t, balls[k * 5 + 1], balls[k * 5 + 2], balls[k * 5 + 3], balls[k * 5 + 4], centers + k * 2, centers + k * 2 + 1);
    }
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            mat[i * h + j] = calcPoint(w - 1, h - 1, i, j, balls, centers, ballCount) > 1;
        }
    }
}

void getRGB(int x0, int y0, int x1, int y1, float t, float mul, int i, int j, int* outr, int* outg, int* outb) {
    t *= mul;
    float x = (float)(i - x0) / (float)(x1 - x0);
    float y = (float)(j - y0) / (float)(y1 - y0);
    float waver = 0.45 * sinf(x * 7.0 + y * 2.0 + t * 5.0) + 0.30 * sinf(y * 9.0 - x * 3.0 - t * 3.7) + 0.25 * sinf((x + y) * 5.0 + sinf(t * 1.3 + y * 4.0) * 1.5);
    float waveg = 0.45 * sinf(y * 8.0 - x * 2.0 + t * 4.3) + 0.30 * sinf(x * 6.0 + y * 5.0 - t * 5.6) + 0.25 * sinf((x - y) * 7.0 + sinf(t * 1.1 + x * 3.0) * 1.5);
    float waveb = 0.45 * sinf((x + y) * 6.0 - t * 4.8) + 0.30 * sinf(x * 10.0 - y * 4.0 + t * 3.3) + 0.25 * sinf((x * 2.0 + y) * 4.0 + sinf(t * 1.5 - y * 5.0) * 1.5);
    float normr = (waver + 1.0) * 0.5;
    float normg = (waveg + 1.0) * 0.5;
    float normb = (waveb + 1.0) * 0.5;
    *outr = (int)(normr * 255.0);
    *outg = (int)(normg * 255.0);
    *outb = (int)(normb * 255.0);
}

void drawContour(bool* buf, char* chrs, int x0, int y0, int x1, int y1, bool* mat, int w, int h, float t, float* balls, float* centers, int ballCount) {
    fillMatrix(mat, w, h, t, balls, centers, ballCount);
    bool last = false;
    for (int j = y0; j <= y1; ++j) {
        for (int i = x0; i <= x1; ++i) {
            int x = i - x0, y = j - y0;
            bool big = false, small = false;
            for (int ax = 0; ax < 2; ++ax) {
                for (int ay = 0; ay < 2; ++ay) {
                    if (mat[(x + ax) * h + (y + ay)]) big = true;
                    else small = true;
                }
            }
            bool set = big && small;
            if (buf[x * (h - 1) + y] != set) {
                buf[x * (h - 1) + y] = set;
                if (!last) ANSI_moveTo(j, i);
                last = true;
                if (set) {
                    int r, g, b;
                    getRGB(x0, y0, x1, y1, t, 10, i, j, &r, &g, &b);
                    ANSI_fgRgb(r, g, b);
                    putchar(chrs[x * (h - 1) + y]);
                } else putchar(' ');
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
    ANSI_clearScreen();
    const int w = x1 - x0 + 2, h = y1 - y0 + 2;
    bool* mat = malloc(w * h * sizeof(bool));
    char* chrs = malloc((w - 1) * (h - 1) * sizeof(char));
    const char chars[8] = {'@', '#', '!', '$', '%', '&', '8', '?'};
    for (int i = 0; i < (w - 1) * (h - 1); ++i) chrs[i] = chars[rand() % 8];
    bool* buf = calloc((w - 1) * (h - 1), sizeof(bool));
    const int ballCount = 25;
    float* balls = generateBalls(ballCount);
    float* centers = malloc(ballCount * 2 * sizeof(float));
    char key;
    for (float t = 0; getchNB(&key) == 0 || key != '\n'; t += 0.001) {
        drawContour(buf, chrs, x0, y0, x1, y1, mat, w, h, t, balls, centers, ballCount);
        fflush(stdout);
        sleepms(16);
    }
    printf(ANSI_RESET);
    ANSI_showCursor();
    putchar('\n');
    fflush(stdout);
    free(balls);
    free(centers);
    free(mat);
    free(chrs);
    free(buf);
    return 0;
}

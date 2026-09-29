#ifndef GRAPHICS_H
#define GRAPHICS_H

void GRAPHICS_getTerminalDimensions(int* x0Out, int* y0Out, int* x1Out, int* y1Out);

void GRAPHICS_draw8(int value);
void GRAPHICS_drawf(float value);

#endif
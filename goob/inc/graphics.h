#ifndef GRAPHICS_H
#define GRAPHICS_H

void GRAPHICS_getTerminalDimensions(int* x0Out, int* y0Out, int* x1Out, int* y1Out);

void GRAPHICS_put8(int value);
void GRAPHICS_putf(float value);

#endif

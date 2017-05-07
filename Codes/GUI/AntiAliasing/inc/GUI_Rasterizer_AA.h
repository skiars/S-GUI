#ifndef __GL_H
#define __GL_H

#include <stdint.h>

typedef struct {
    int  x;
    int  len;
    uint8_t* covers;
} Span;

typedef void (*render_span_func)(int y, Span *span);

typedef struct TCell_ {
    int x;
    int cover;
    int area;
    struct TCell_ *next;
} TCell;

typedef struct {
    int ex, ey;
    int min_ex, max_ex;
    int min_ey, max_ey;

    int x, y;

    int area;
    int cover;

    TCell *cells;
    TCell **ycells;
    int  max_cells;
    int  num_cells;

    void (*fill_span)(int x0, int y0, int x1);
    void (*blend_pixel)(int x0, int y0, unsigned alpha);
} Rasterizer;

typedef struct {
    int min_x;
    int last_x;
    int y;

    Span *span;
} Scanline;

Rasterizer *render_init(void);
void render_free(Rasterizer *ras);
void GUI_MoveTo(Rasterizer *ras, int x, int y);
void GUI_LineTo(Rasterizer *ras, int to_x, int to_y);
void sweep_scanlines(Rasterizer *ras);

#endif

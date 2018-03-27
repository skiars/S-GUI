#ifndef __GUI_STROKELINE_H
#define __GUI_STROKELINE_H

#include "GUI_AATypedef.h"
#include "GUI_Rasterizer.h"

typedef struct {
    Vertex vl1, vl2;  // vertex line 1, vertex line 2
    Vertex v1, v2;
    pos_t width;
    unsigned cmd;
    GUI_Rasterizer *ras;
} StrokeLine;

void line_set_to(StrokeLine *vc, pos_t x, pos_t y);
void line_move_to(GUI_Rasterizer * ras, StrokeLine *vc, pos_t x, pos_t y);
void line_stoke_end(GUI_Rasterizer * ras, StrokeLine *vc);

#endif

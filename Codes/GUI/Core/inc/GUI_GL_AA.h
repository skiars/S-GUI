#ifndef __GUI_GL_AA_H
#define __GUI_GL_AA_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"

void GUI_DrawLineAA(int x0, int y0, int x1, int y1);
void GUI_FillPolygonAA(GUI_POINT *Points, int cnt);
void GUI_DrawLinesAA(int x, int y, GUI_POINT *Points, int cnt);
void GUI_FillCircleAA(int x0, int y0, int r);

#endif

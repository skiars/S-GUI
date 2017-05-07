#ifndef __GUI_GL_H
#define __GUI_GL_H

#include "GUI_Typedef.h"

#define GUI_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define GUI_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define GUI_ABS(a)              (((a) >  0 ) ? (a) : -(a))

void GUI_DrawPoint(int x, int y);
void GUI_VertLine(int x0, int y0, int len);
void GUI_HoriLine(int x0, int y0, int len);
void GUI_DrawRect(int x0, int y0, int width, int height);
void GUI_FillRect(int x0, int y0, int width, int height);
void GUI_DrawLine(int x0, int y0, int x1, int y1);
void GUI_DrawCircle(int x0, int y0, int r);
void GUI_FillCircle(int x0, int y0, int r);
void GUI_DrawPolygon(GUI_POINT *Points, int cnt);
void GUI_FillPolygon(GUI_POINT *Points, int cnt);
void GUI_GetPolyArea(GUI_RECT *r, GUI_POINT *Points, int cnt);
void GUI_DrawLines(int x, int y, GUI_POINT *Points, int cnt);

#endif /* __GUI_2D_H */

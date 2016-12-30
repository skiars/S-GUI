#ifndef __GUI_GL_H
#define __GUI_GL_H

#include "GUI_Typedef.h"

#define GUI_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define GUI_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define GUI_ABS(a)              (((a) >  0 ) ? (a) : -(a))

#define CLIP_X(X0, X1) \
{ \
    X0 = X0 > _ClipRect.x0 ? X0 : _ClipRect.x0; \
    X1 = X1 < _ClipRect.x1 ? X1 : _ClipRect.x1; \
}

#define CLIP_Y(Y0, Y1) \
{ \
    Y0 = Y0 > _ClipRect.y0 ? Y0 : _ClipRect.y0; \
    Y1 = Y1 < _ClipRect.y1 ? Y1 : _ClipRect.y1; \
}

#define CHECK_X(X) \
    { if ((X) > _ClipRect.x1 || (X) < _ClipRect.x0) return; }

#define CHECK_Y(Y) \
    { if ((Y) > _ClipRect.y1 || (Y) < _ClipRect.y0) return; }

void GUI_DrawPoint(int x, int y);
void GUI_VertLine(int x0, int y0, int len);
void GUI_HoriLine(int x0, int y0, int len);
void GUI_DrawRect(int x0, int y0, int xSize, int ySize);
void GUI_FillRect(int x0, int y0, int xSize, int ySize);
void GUI_DrawLine(int x0, int y0, int x1, int y1);
void GUI_DrawCircle(int x0, int y0, int r);
void GUI_FillCircle(int x0, int y0, int r);
void GUI_DrawPolygon(GUI_POINT *Points, int cnt);
void GUI_FillPolygon(GUI_POINT *Points, int cnt);
void GUI_GetPolyArea(GUI_RECT *r, GUI_POINT *Points, int cnt);
void GUI_DrawLines(int x, int y, GUI_POINT *Points, int cnt);

#endif /* __GUI_2D_H */

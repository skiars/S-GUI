#ifndef __GUI_2D_H
#define __GUI_2D_H

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

void GUI_DrawPoint(i_16 x,i_16 y,GUI_COLOR Color); /* 绘制点 */
void GUI_VertLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color); /* 画垂直线 */
void GUI_HoriLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color); /* 画水平线 */
void GUI_DrawRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color); /* 画矩形框 */
void GUI_FillRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color); /* 填充矩形 */
void GUI_DrawLine(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color);

#endif /* __GUI_2D_H */

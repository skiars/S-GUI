#ifndef __GUI_2D_H
#define __GUI_2D_H

#include "GUI_Typedef.h"

#define GUI_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define GUI_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define GUI_ABS(a)              (((a) >  0 ) ? (a) : -(a))

#define CHECK_X(a, b) \
    { if ((a) > _PaintArea->x1 || (b) < _PaintArea->x0) return; }

#define CHECK_Y(a, b) \
    { if ((a) > _PaintArea->y1 && (b) < _PaintArea->y0) return; }

u_16 GUI_RGB888To565(u_32 rgb);
u_32 GUI_RGB565To888(u_16 rgb);
GUI_COLOR GUI_ReadPoint(i_16 x, i_16 y); /* 读取点 */
void GUI_DrawPoint(i_16 x,i_16 y,GUI_COLOR Color); /* 绘制点 */
void GUI_VertLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color); /* 画垂直线 */
void GUI_HoriLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color); /* 画水平线 */
void GUI_DrawRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color); /* 画矩形框 */
void GUI_FillRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color); /* 填充矩形 */

#endif

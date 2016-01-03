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
void GUI_DrawTailorPoint(i_16 x,i_16 y,GUI_COLOR Color);
GUI_COLOR GUI_ReadTailorPoint(i_16 x, i_16 y);
GUI_COLOR GUI_AlphaBlend(GUI_COLOR Color1, GUI_COLOR Color2, u_16 Alpha);
void GUI_DrawTailorPoint(i_16 x,i_16 y,GUI_COLOR Color); /* »­²Ã¼ôµã */
void GUI_VertLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color); /* »­´¹Ö±Ïß */
void GUI_VertTailorLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color); /* »­´¹Ö±²Ã¼ôÏß */
void GUI_HoriLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color); /* »­Ë®Æ½Ïß */
void GUI_HoriTailorLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color); /* »­Ë®Æ½²Ã¼ôÏß */
void GUI_DrawRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color); /* »­¾ØÐÎ¿ò */
void GUI_DrawTailorRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color); /* »­²Ã¼ô¾ØÐÎ¿ò */
void GUI_FillRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color); /* Ìî³ä¾ØÐÎ */
void GUI_FillTailorRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color); /* Ìî³ä²Ã¼ô¾ØÐÎ */
void GUI_DrawArc(u_16 sx,u_16 sy,u_16 ex,u_16 ey,u_16 rx,u_16 ry,u_16 r,GUI_COLOR Color,u_8 mode); /* »­Ô²»¡ */

#endif

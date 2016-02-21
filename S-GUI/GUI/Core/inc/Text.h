#ifndef __TEXT_H
#define __TEXT_H

#include "GUI_Typedef.h"
#include "GUI_Font.h"

u_16 GetStringPixel(const char *str, GUI_FontType Font);
u_16 GUI_SkipWord(const char *str, GUI_FontType Font, u_16 x_pix, u_16 *len);
void GUI_DispStringCurRect(i_16 x0, i_16 y0, const char *str,
                           GUI_COLOR Color, GUI_FontType Font);
void GUI_DispStringCurRectN(i_16 x0, i_16 y0, const char *str,
                           GUI_COLOR Color, GUI_FontType Font, u_16 n);
void GUI_DspStringCurRectMiddle(i_16 x0, i_16 y0, i_16 xSize,
                                i_16 ySize, const char *str,
                                GUI_COLOR Color, GUI_FontType Font);
void GUI_DispStringAt(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize,
                      const char *str, GUI_COLOR Color, GUI_FontType Font);
void GUI__DispDebugInfo(const char *str);

#endif
